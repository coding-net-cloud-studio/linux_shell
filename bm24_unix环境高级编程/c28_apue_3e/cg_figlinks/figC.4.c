#include <stdio.h>   // 包含标准输入输出库
#include <stdlib.h>  // 包含标准库函数
#include <string.h>  // 包含字符串处理函数
#include <errno.h>   // 包含错误处理头文件
#include "apue.h"    // 引入自定义的apue库,可能包含一些实用函数

/*
 * 我们内部用于跟踪内存流的结构体
 */
struct memstream
{
    char  *buf;    /* 内存缓冲区 */
    size_t rsize;  /* 缓冲区的实际大小 */
    size_t vsize;  /* 缓冲区的虚拟大小 */
    size_t curpos; /* 缓冲区中的当前位置 */
    int    flags;  /* 标志位,具体含义见下文 */
};

/* 标志定义 */
#define MS_READ     0x01 /* 打开用于读取 */
#define MS_WRITE    0x02 /* 打开用于写入 */
#define MS_APPEND   0x04 /* 追加到流 */
#define MS_TRUNCATE 0x08 /* 打开时截断流 */
#define MS_MYBUF    0x10 /* 关闭时释放缓冲区 */

/* 定义MIN宏,返回两个参数中较小的一个 */
#ifndef MIN
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif

/* 函数声明及标准文档注释 */
/**
 * @brief 从流中读取数据
 * @param void * 流的指针
 * @param char * 目标缓冲区
 * @param int 要读取的字节数
 * @return int 实际读取的字节数
 */
static int mstream_read(void *, char *, int);

/**
 * @brief 向流中写入数据
 * @param void * 流的指针
 * @param const char * 源缓冲区
 * @param int 要写入的字节数
 * @return int 实际写入的字节数
 */
static int mstream_write(void *, const char *, int);

/**
 * @brief 在流中移动位置
 * @param void * 流的指针
 * @param fpos_t 偏移量
 * @param int 寻址模式(如SEEK_SET, SEEK_CUR, SEEK_END)
 * @return fpos_t 新的位置
 */
static fpos_t mstream_seek(void *, fpos_t, int);

/**
 * @brief 关闭流
 * @param void * 流的指针
 * @return int 关闭操作的结果
 */
static int mstream_close(void *);

/**
 * @brief 将类型字符串转换为标志位
 * @param const char *__restrict type 类型字符串
 * @return int 标志位组合
 */
static int type_to_flags(const char *__restrict type);

/**
 * @brief 查找缓冲区末尾的位置
 * @param char * 缓冲区指针
 * @param size_t 缓冲区长度
 * @return off_t 缓冲区末尾的位置
 */
static off_t find_end(char *buf, size_t len);

// fmemopen函数用于创建一个流,该流与内存缓冲区关联.
// 这允许将内存区域当作文件一样进行读写操作.
//
// @param buf 指向要关联的内存缓冲区的指针.
// @param size 缓冲区的大小,以字节为单位.
// @param type 流的类型,例如 "r" 表示只读,"w" 表示只写,"a" 表示追加.
// @return 如果成功,返回一个FILE指针;如果失败,返回NULL.
FILE *
fmemopen(void *__restrict buf, size_t size, const char *__restrict type)
{
    struct memstream *ms;  // 定义一个memstream结构体指针,用于存储流的状态.
    FILE             *fp;  // 定义一个FILE指针,用于返回创建的流.

    // 此处应添加代码来初始化memstream结构体,并根据type参数设置流的读写模式.
    // 然后,将memstream结构体与FILE指针关联起来.

    // 返回创建的FILE指针.

    // 检查传入的size是否为0,如果是,则设置errno为EINVAL并返回NULL
    if (size == 0)
    {
        // 设置errno为EINVAL,表示传递了无效的参数
        // 返回NULL,表明函数执行失败
        errno = EINVAL;
        return (NULL);
    }

    // 尝试为memstream结构体分配内存,如果失败,则设置errno为ENOMEM并返回NULL
    if ((ms = malloc(sizeof(struct memstream))) == NULL)
    {
        // 当内存分配失败时,设置errno为ENOMEM,并返回NULL
        // errno是一个全局变量,用于存储系统调用的错误代码
        // ENOMEM表示内存不足的错误
        errno = ENOMEM;
        return (NULL);
    }

    // 将type转换为对应的标志位,如果转换失败(返回0),则设置errno为EINVAL,释放之前分配的内存,并返回NULL
    if ((ms->flags = type_to_flags(type)) == 0)
    {
        // 当errno被设置为EINVAL时,表示传递了一个无效的参数.
        // 接下来,释放之前分配的内存ms.
        // 最后,返回NULL指针,表示操作失败.
        errno = EINVAL;
        free(ms);
        return (NULL);
    }

    if (buf == NULL)
    {
        // 检查内存块的标志是否同时包含读和写权限
        if ((ms->flags & (MS_READ | MS_WRITE)) != (MS_READ | MS_WRITE))
        {
            // 如果不同时包含读和写权限,设置错误码并释放内存块,返回NULL
            // 当errno被设置为EINVAL时,表示传递了一个无效的参数.
            // 接下来释放ms指针所指向的内存,防止内存泄漏.
            // 最后返回NULL,表示函数执行失败.
            errno = EINVAL;
            free(ms);
            return (NULL);
        }
        // 尝试为内存块分配指定大小的内存
        if ((ms->buf = malloc(size)) == NULL)
        {
            // 如果分配失败,设置错误码并释放内存块,返回NULL
            errno = ENOMEM;
            free(ms);
            return (NULL);
        }
        // 设置内存块的实际大小
        ms->rsize = size;
        // 更新标志位,表示内存块有自己的缓冲区
        ms->flags |= MS_MYBUF;
        // 初始化当前位置指针
        ms->curpos = 0;
    }
    else
    {
        // 设置内存流结构体的缓冲区指针和大小
        // 设置ms结构体中的buf指针为传入的buf变量,用于存储数据
        // 设置ms结构体中的rsize变量为传入的size值,表示读取数据的大小
        ms->buf   = buf;
        ms->rsize = size;

        // 如果内存流的标志位包含追加模式,则将当前位置设置为缓冲区的末尾
        // 否则,将当前位置设置为0
        if (ms->flags & MS_APPEND)
        {
            // 如果ms的标志位包含MS_APPEND,表示需要追加内容到缓冲区末尾
            // find_end函数用于查找缓冲区的末尾位置
            // ms->buf是缓冲区的起始地址,ms->rsize是缓冲区的大小
            // 函数返回值是缓冲区中最后一个可写位置的指针
            ms->curpos = find_end(ms->buf, ms->rsize);
        }
        else
        {
            // 如果ms的标志位不包含MS_APPEND,表示需要从头开始写入缓冲区
            // 因此将当前位置设置为缓冲区的起始位置
            ms->curpos = 0;
        }
    }

    // 检查文件打开模式标志,并根据模式设置文件大小
    if (ms->flags & MS_APPEND)
    {
        // 如果是追加模式("a"模式),将文件大小设置为当前位置
        // 这意味着新的数据将被添加到文件的末尾
        ms->vsize = ms->curpos;
    }
    else if (ms->flags & MS_TRUNCATE)
    {
        // 如果是截断模式("w"模式),将文件大小设置为0
        // 这意味着文件将被清空,之前的内容将丢失
        ms->vsize = 0;
    }
    else
    {
        // 默认情况下,假设是读取模式("r"模式),文件大小保持不变
        // 这意味着文件将被打开用于读取,不会更改其大小
        ms->vsize = size;
    }

    // 使用funopen函数打开一个流,该函数需要传入一个结构体指针ms以及四个函数指针:
    // mstream_read - 读取函数
    // mstream_write - 写入函数
    // mstream_seek - 定位函数
    // mstream_close - 关闭函数
    fp = funopen(ms, mstream_read, mstream_write, mstream_seek, mstream_close);

    // 检查fp是否为NULL,如果是,则表示打开流失败
    if (fp == NULL)
    {
        // 如果ms结构体中的flags字段包含MS_MYBUF标志,则释放ms中的buf指针指向的内存
        if (ms->flags & MS_MYBUF)
            free(ms->buf);

        // 释放ms结构体本身占用的内存
        free(ms);
    }

    // 返回文件指针fp
    return (fp);
}

// 该函数用于将类型字符串转换为相应的标志位
// 参数:
//   type - 指向类型字符串的指针
// 返回值:
//   转换后的标志位
static int
type_to_flags(const char *__restrict type)
{
    const char *cp;         // 定义字符指针cp,用于遍历传入的字符串
    int         flags = 0;  // 初始化标志flags为0,用于存储文件打开模式

    for (cp = type; *cp != 0; cp++)  // 遍历字符串直到遇到字符串结束符'\0'
    {
        switch (*cp)  // 根据当前字符设置文件打开模式
        {
        case 'r':              // 读模式
            if (flags != 0)    // 如果flags不为0,表示已经有模式设置,返回错误
                return (0);    /* error */
            flags |= MS_READ;  // 设置读模式
            break;

        case 'w':                             // 写模式
            if (flags != 0)                   // 如果flags不为0,表示已经有模式设置,返回错误
                return (0);                   /* error */
            flags |= MS_WRITE | MS_TRUNCATE;  // 设置写模式和截断模式
            break;

        case 'a':                // 追加模式
            if (flags != 0)      // 如果flags不为0,表示已经有模式设置,返回错误
                return (0);      /* error */
            flags |= MS_APPEND;  // 设置追加模式
            break;

        case '+':                         // 读写模式
            if (flags == 0)               // 如果flags为0,表示没有模式设置,返回错误
                return (0);               /* error */
            flags |= MS_READ | MS_WRITE;  // 设置读写模式
            break;

        case 'b':            // 二进制模式
            if (flags == 0)  // 如果flags为0,表示没有模式设置,返回错误
                return (0);  /* error */
            // 二进制模式不需要额外设置flags,因此这里不改变flags
            break;

        default:        // 默认情况,非法字符
            return (0); /* error */
        }
    }
    return (flags);  // 返回设置好的文件打开模式标志
}

/*
 * 函数功能:查找缓冲区中字符串的结束位置
 * 参数:
 *   buf - 指向要检查的字符数组的指针
 *   len - buf数组的长度
 * 返回值:
 *   如果找到字符串结束符'\0',返回其在buf中的偏移量;
 *   如果没有找到,返回-1.
 */
static off_t
find_end(char *buf, size_t len)
{
    /* 初始化偏移量为0 */
    off_t off = 0;

    /* 遍历缓冲区直到找到终止符或到达缓冲区末尾 */
    while (off < len)
    {
        /* 如果当前字符是终止符,则跳出循环 */
        if (buf[off] == 0)
            break;
        /* 否则,增加偏移量 */
        off++;
    }
    /* 返回找到的终止符的位置 */
    return (off);
}

// mstream_read 函数用于从流中读取数据到缓冲区.
// 参数:
//   cookie: 一个指向包含流状态信息的结构的指针.
//   buf: 目标缓冲区,数据将被读取到这里.
//   len: 要读取的最大字节数.
// 返回值:
//   成功时返回读取的字节数,如果到达流的末尾或发生错误则返回-1.
static int
mstream_read(void *cookie, char *buf, int len)
{
    // 定义读取的字节数
    int nr;
    // 将cookie转换为memstream结构体指针
    struct memstream *ms = cookie;

    // 检查流是否已打开用于读取
    if (!(ms->flags & MS_READ))
    {
        errno = EBADF;  // 设置错误码
        return (-1);    // 返回错误
    }
    // 如果当前位置已经超过流的虚拟大小,则返回0表示没有更多数据可读
    // 检查当前位置是否超过了向量大小
    // 如果当前位置大于或等于向量大小,则返回0
    if (ms->curpos >= ms->vsize)
        return (0);

    // 计算实际可以读取的字节数,不能超过请求的长度和剩余的流大小
    nr = MIN(len, ms->vsize - ms->curpos);
    // 将数据从流缓冲区复制到提供的缓冲区
    memcpy(buf, ms->buf + ms->curpos, nr);
    // 更新当前位置指针
    ms->curpos += nr;
    // 返回实际读取的字节数
    return (nr);
}

// mstream_write 函数用于将数据写入到流中.
// 参数:
//   cookie: 一个指向与流相关的私有数据的指针.
//   buf: 包含要写入数据的缓冲区的指针.
//   len: 要写入的字节数.
// 返回值:
//   成功时返回写入的字节数,失败时返回负数.
static int
mstream_write(void *cookie, const char *buf, int len)
{

    // 定义变量nw和off,分别表示写入的字节数和写入的偏移量
    int nw, off;

    // 将cookie转换为memstream结构体指针
    struct memstream *ms = cookie;

    // 检查memstream结构体的flags,如果不包含MS_APPEND或MS_WRITE标志,则设置errno为EBADF并返回-1
    // 检查文件状态标志,确保文件是可追加或可写的
    // 如果不是,设置errno为EBADF(错误的文件描述符),并返回-1
    if (!(ms->flags & (MS_APPEND | MS_WRITE)))
    {
        errno = EBADF;  // 设置错误码表示无效的文件描述符
        return (-1);    // 返回-1表示操作失败
    }

    // 如果flags包含MS_APPEND标志,则将偏移量off设置为当前文件大小vsize
    if (ms->flags & MS_APPEND)
        off = ms->vsize;
    // 否则,将偏移量off设置为当前位置curpos
    else
        off = ms->curpos;

    // 计算实际写入的字节数nw,取len和剩余可写空间(ms->rsize - off)的较小值
    nw = MIN(len, ms->rsize - off);

    // 将buf中的数据复制到memstream的缓冲区ms->buf的off位置,并写入nw个字节
    memcpy(ms->buf + off, buf, nw);

    // 更新当前位置curpos为off加上写入的字节数nw
    ms->curpos = off + nw;

    // 当前位置大于虚拟大小时,更新虚拟大小
    if (ms->curpos > ms->vsize)
    {
        ms->vsize = ms->curpos;
        // 如果同时具有读写权限且虚拟大小小于实际大小,则在缓冲区末尾添加空字符
        // 检查内存块的标志是否同时包含读和写权限,并且虚拟大小小于实际大小
        // 如果条件满足,则在缓冲区的末尾添加一个空字节以确保数据完整性
        if (((ms->flags & (MS_READ | MS_WRITE)) == (MS_READ | MS_WRITE)) &&
            (ms->vsize < ms->rsize))
            *(ms->buf + ms->vsize) = 0;  // 在缓冲区的当前虚拟大小位置后添加一个空字节
    }

    // 如果只有写或追加权限且没有读权限
    if ((ms->flags & (MS_WRITE | MS_APPEND)) &&
        !(ms->flags & MS_READ))
    {
        // 如果当前位置小于实际大小,则在当前位置添加空字符
        if (ms->curpos < ms->rsize)
            *(ms->buf + ms->curpos) = 0;
        // 否则在实际大小的最后一个位置添加空字符
        else
            *(ms->buf + ms->rsize - 1) = 0;
    }

    // 返回写入的字节数
    return (nw);
}

/*
在这段代码中,mstream_seek函数用于改变内存流memstream的当前读写位置.
函数接收三个参数:一个指向memstream结构体的指针cookie,
一个表示偏移量的fpos_t类型的pos,
以及一个表示偏移起始位置的整数whence.
函数根据whence的值计算新的位置,
并更新memstream结构体中的当前位置curpos.
如果计算出的新位置不合法,
函数将设置错误码errno并返回-1.
*/
// static fpos_t

// NOTE wmtag_memo_如下的函数被我修改了_与原始内容不一致_开始
// mstream_seek 函数用于在流中移动位置指针.
// 参数:
//   cookie: 流的上下文或状态信息的指针.
//   pos: 想要移动到的位置.
//   whence: 移动的起始点,可以是 SEEK_SET(从文件开头),SEEK_CUR(从当前位置)或 SEEK_END(从文件末尾).
static int
mstream_seek(void *cookie, fpos_t pos, int whence)
{
    // 定义偏移量变量
    int off;
    // 将cookie转换为memstream结构体指针
    struct memstream *ms = cookie;

    // 根据whence参数确定偏移量的起始位置
    switch (whence)
    {
    case SEEK_SET:  // 从文件开始位置计算偏移
        off = pos.__pos;
        break;
    case SEEK_END:  // 从文件末尾计算偏移
        off = ms->vsize + pos.__pos;
        break;
    case SEEK_CUR:  // 从当前位置计算偏移
        off = ms->curpos + pos.__pos;
        break;
    }
    // 检查偏移量是否合法
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
// NOTE wmtag_memo_如下的函数被我修改了_与原始内容不一致_结束

// mstream_close - 关闭与给定cookie关联的多流
// @param cookie: 指向与流关联的私有数据的指针
// @return: 成功返回0,失败返回非0值
static int
mstream_close(void *cookie)
{
    /**
     * @brief 关闭内存流并释放相关资源
     *
     * @param cookie 指向memstream结构的指针
     * @return int 成功返回0
     */
    struct memstream *ms = cookie;  // 将cookie转换为memstream结构指针

    if (ms->flags & MS_MYBUF)  // 检查是否使用了自定义缓冲区
        free(ms->buf);         // 释放自定义缓冲区
    free(ms);                  // 释放memstream结构
    return (0);                // 返回成功
}
