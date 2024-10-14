#include <stdio.h>   // 包含标准输入输出库
#include <stdlib.h>  // 包含标准库函数
#include <string.h>  // 包含字符串处理函数
#include <errno.h>   // 包含错误处理头文件

// 以下是添加注释后的代码,由于原代码段没有具体的函数实现,所以注释将针对每个头文件的作用进行说明.

// stdio.h 提供了标准的输入输出功能,如 printf 和 scanf 等.
// stdlib.h 提供了内存分配函数(如 malloc 和 free)和其他标准库函数.
// string.h 提供了字符串处理函数,如 strcpy, strcmp 等.
// errno.h 定义了错误码,用于错误处理.

/*
 * 我们内部跟踪内存流的结构
 */
struct memstream
{
    char  *buf;     // 内存中的缓冲区
    size_t rsize;   // 缓冲区的实际大小
    size_t vsize;   // 缓冲区的虚拟大小
    size_t curpos;  // 缓冲区中的当前位置
    int    flags;   // 见下文
};

/* 标志定义 */
#define MS_READ     0x01 /* 打开文件用于读取 */
#define MS_WRITE    0x02 /* 打开文件用于写入 */
#define MS_APPEND   0x04 /* 追加到文件流 */
#define MS_TRUNCATE 0x08 /* 打开文件时截断文件流 */
#define MS_MYBUF    0x10 /* 关闭时释放缓冲区 */

/* 定义宏MIN,返回两个参数中较小的一个 */
#ifndef MIN
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif

/* 函数声明 */
/**
 * @brief 从内存流中读取数据
 * @param ptr 内存流的指针
 * @param buf 用于存储读取数据的缓冲区
 * @param size 要读取的字节数
 * @return 实际读取的字节数
 */
static int mstream_read(void *, char *, int);

/**
 * @brief 向内存流中写入数据
 * @param ptr 内存流的指针
 * @param buf 包含要写入数据的缓冲区
 * @param size 要写入的字节数
 * @return 实际写入的字节数
 */
static int mstream_write(void *, const char *, int);

/**
 * @brief 在内存流中移动读写位置
 * @param ptr 内存流的指针
 * @param offset 位置偏移量
 * @param whence 位置参考点(如SEEK_SET, SEEK_CUR, SEEK_END)
 * @return 新的文件位置指示器
 */
static fpos_t mstream_seek(void *, fpos_t, int);

/**
 * @brief 关闭内存流并释放资源
 * @param ptr 内存流的指针
 * @return 关闭操作的结果状态码
 */
static int mstream_close(void *);

/**
 * @brief 将字符串类型的标志转换为对应的文件打开标志
 * @param type 文件打开类型的字符串表示
 * @return 对应的文件打开标志
 */
static int type_to_flags(const char *__restrict type);

/**
 * @brief 查找缓冲区中字符串的结束位置
 * @param buf 缓冲区的指针
 * @param len 缓冲区的长度
 * @return 字符串的结束位置
 */
static off_t find_end(char *buf, size_t len);

/*
 * fmemopen - 使用内存缓冲区作为文件流
 *
 * @buf: 指向内存缓冲区的指针,如果为NULL,则会分配一个新的缓冲区
 * @size: 缓冲区的大小
 * @type: 文件流的模式,如 "r", "w", "a" 等
 *
 * 返回: 成功时返回FILE指针,失败时返回NULL
 */
FILE *
fmemopen(void *__restrict buf, size_t size, const char *__restrict type)
{
    struct memstream *ms;  // 内存流结构体指针
    FILE             *fp;  // 文件流指针

    // 检查缓冲区大小是否为0
    if (size == 0)
    {
        errno = EINVAL;
        return (NULL);
    }
    // 分配内存流结构体内存
    // 分配内存以创建一个新的memstream结构体实例
    // 如果malloc调用失败,返回NULL,并将errno设置为ENOMEM表示内存不足
    if ((ms = malloc(sizeof(struct memstream))) == NULL)
    {
        errno = ENOMEM;
        return (NULL);
    }

    // 将文件流模式字符串转换为标志位
    // 如果类型转换后的标志为0,则设置errno为EINVAL,释放ms指向的内存,并返回NULL
    if ((ms->flags = type_to_flags(type)) == 0)
    {
        errno = EINVAL;  // 设置错误码为无效参数
        free(ms);        // 释放内存
        return (NULL);   // 返回空指针
    }

    // 如果buf为NULL,则分配新的缓冲区
    if (buf == NULL)
    {
        // 检查模式是否同时包含读写权限
        // 检查内存流(ms)的标志位,确保同时设置了读(MS_READ)和写(MS_WRITE)权限
        if ((ms->flags & (MS_READ | MS_WRITE)) != (MS_READ | MS_WRITE))
        {
            // 如果不是同时设置了读和写权限,则设置errno为EINVAL表示无效参数
            errno = EINVAL;
            // 释放内存流占用的内存
            free(ms);
            // 返回NULL,表示操作失败
            return (NULL);
        }

        // 分配新的缓冲区内存
        // 分配内存空间给缓冲区
        if ((ms->buf = malloc(size)) == NULL)
        {
            // 如果分配失败,设置错误码并释放已分配的内存
            errno = ENOMEM;
            free(ms);
            // 返回空指针表示失败
            return (NULL);
        }
        // 设置缓冲区的大小
        ms->rsize = size;
        // 设置标志位,表示使用了自定义缓冲区
        ms->flags |= MS_MYBUF;
        // 初始化当前位置为0
        ms->curpos = 0;
    }
    else
    {
        ms->buf   = buf;   // 将传入的缓冲区指针赋值给内存流结构体的buf成员
        ms->rsize = size;  // 将传入的大小赋值给内存流结构体的rsize成员
        // 如果模式包含追加,则将当前位置设置为缓冲区末尾
        if (ms->flags & MS_APPEND)
            ms->curpos = find_end(ms->buf, ms->rsize);  // 如果标志位中包含追加模式,则调用find_end函数找到缓冲区的末尾,并将当前位置设置为缓冲区末尾
        else
            ms->curpos = 0;  // 如果不包含追加模式,则将当前位置设置为0
    }
    // 根据文件流模式设置可见大小
    if (ms->flags & MS_APPEND)
    { /* "a" 模式 */
        ms->vsize = ms->curpos;
    }
    else if (ms->flags & MS_TRUNCATE)
    { /* "w" 模式 */
        ms->vsize = 0;
    }
    else
    { /* "r" 模式 */
        ms->vsize = size;
    }
    // 使用自定义的内存流操作函数创建文件流
    fp = funopen(ms, mstream_read, mstream_write, mstream_seek, mstream_close);
    // 如果文件流创建失败,则释放已分配的内存
    // 检查文件指针fp是否为NULL
    if (fp == NULL)
    {
        // 如果ms结构体的flags成员与MS_MYBUF标志位有交集,表示ms->buf是动态分配的,需要释放
        if (ms->flags & MS_MYBUF)
            free(ms->buf);
        // 释放ms结构体占用的内存
        free(ms);
    }
    // 返回文件指针fp
    return (fp);
}

static int
type_to_flags(const char *__restrict type)  // 将文件打开模式字符串转换为对应的标志位
{
    const char *cp;         // 指向当前处理的字符
    int         flags = 0;  // 初始化标志位为0

    for (cp = type; *cp != 0; cp++)  // 遍历模式字符串
    {
        switch (*cp)  // 根据当前字符进行相应的处理
        {
        case 'r':            // 只读模式
            if (flags != 0)  // 如果已经有其他模式,则返回错误
                return (0);
            flags |= MS_READ;  // 设置读标志位
            break;

        case 'w':            // 写入模式,如果文件存在则清空
            if (flags != 0)  // 如果已经有其他模式,则返回错误
                return (0);
            flags |= MS_WRITE | MS_TRUNCATE;  // 设置写标志位和截断标志位
            break;

        case 'a':            // 追加模式
            if (flags != 0)  // 如果已经有其他模式,则返回错误
                return (0);
            flags |= MS_APPEND;  // 设置追加标志位
            break;

        case '+':            // 读写模式
            if (flags == 0)  // 如果之前没有设置任何模式,则返回错误
                return (0);
            flags |= MS_READ | MS_WRITE;  // 设置读写标志位
            break;

        case 'b':            // 二进制模式
            if (flags == 0)  // 如果之前没有设置任何模式,则返回错误
                return (0);
            break;  // 二进制模式不需要设置额外的标志位

        default:         // 遇到未知模式字符
            return (0);  // 返回错误
        }
    }
    return (flags);  // 返回最终的标志位组合
}

static off_t
find_end(char *buf, size_t len)
{
    /* 初始化偏移量为0 */
    off_t off = 0;

    /* 遍历缓冲区直到找到终止符或达到缓冲区长度 */
    while (off < len)
    {
        /* 如果当前字符是终止符,则跳出循环 */
        if (buf[off] == 0)
            break;
        /* 否则,偏移量加1 */
        off++;
    }
    /* 返回找到的终止符的位置 */
    return (off);
}

static int
mstream_read(void *cookie, char *buf, int len)
{
    // 定义局部变量nr,用于存储实际读取的字节数
    int nr;
    // 将cookie转换为memstream结构体指针ms
    struct memstream *ms = cookie;

    // 检查标志位,如果不是读模式,则返回错误
    if (!(ms->flags & MS_READ))
    {
        errno = EBADF;
        return (-1);
    }
    // 如果当前位置已经超过缓冲区大小,则表示已经读取完毕,返回0
    if (ms->curpos >= ms->vsize)
        return (0);

    // 计算实际可以读取的字节数,不能超过缓冲区剩余大小
    nr = MIN(len, ms->vsize - ms->curpos);
    // 将缓冲区中的数据复制到buf中
    memcpy(buf, ms->buf + ms->curpos, nr);
    // 更新当前读取位置
    ms->curpos += nr;
    // 返回实际读取的字节数
    return (nr);
}
static int
mstream_read(void *cookie, char *buf, int len)
{
    // 定义局部变量nr,用于存储实际读取的字节数
    int nr;
    // 将cookie转换为memstream结构体指针ms
    struct memstream *ms = cookie;

    // 检查标志位,如果不是读模式,则返回错误
    if (!(ms->flags & MS_READ))
    {
        errno = EBADF;
        return (-1);
    }
    // 如果当前位置已经超过缓冲区大小,则表示已经读取完毕,返回0
    if (ms->curpos >= ms->vsize)
        return (0);

    // 计算实际可以读取的字节数,不能超过缓冲区剩余大小
    nr = MIN(len, ms->vsize - ms->curpos);
    // 将缓冲区中的数据复制到buf中
    memcpy(buf, ms->buf + ms->curpos, nr);
    // 更新当前读取位置
    ms->curpos += nr;
    // 返回实际读取的字节数
    return (nr);
}

static fpos_t
mstream_seek(void *cookie, fpos_t pos, int whence)
{
    // 定义偏移量变量
    int off;
    // 将cookie转换为memstream结构体指针
    struct memstream *ms = cookie;

    // 根据whence参数计算偏移量
    switch (whence)
    {
    case SEEK_SET:  // 从文件开始位置计算偏移
        off = pos;
        break;
    case SEEK_END:  // 从文件末尾计算偏移
        off = ms->vsize + pos;
        break;
    case SEEK_CUR:  // 从当前位置计算偏移
        off = ms->curpos + pos;
        break;
    }
    // 检查偏移量是否有效
    if (off < 0 || off > ms->vsize)
    {
        errno = EINVAL;  // 设置错误码
        return -1;       // 返回错误
    }
    // 更新当前位置
    ms->curpos = off;
    // 返回新的位置
    return (off);
}

// mstream_close函数用于关闭内存流并释放相关资源.
// 参数:
//   cookie: 指向memstream结构体的指针,该结构体包含了内存流的相关信息.
//
// 返回值:
//   成功时返回0.
static int
mstream_close(void *cookie)
{
    // 将cookie转换为memstream结构体指针
    struct memstream *ms = cookie;

    // 如果ms的flags成员包含MS_MYBUF标志,说明缓冲区是动态分配的,需要释放
    if (ms->flags & MS_MYBUF)
        free(ms->buf);

    // 释放memstream结构体本身占用的内存
    free(ms);

    // 返回成功状态码
    return (0);
}
