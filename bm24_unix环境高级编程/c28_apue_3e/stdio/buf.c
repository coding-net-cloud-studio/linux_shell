#include "apue.h"

// 函数:pr_stdio
// 功能:打印与stdio相关的文件状态信息
// 参数:
//   - const char *:要打印的信息前缀
//   - FILE *:文件指针
void pr_stdio(const char *, FILE *);

// 函数:is_unbuffered
// 功能:检查文件流是否未缓冲
// 参数:
//   - FILE *:文件指针
// 返回值:
//   - int:如果文件流未缓冲返回非零值,否则返回零
int is_unbuffered(FILE *);

// 函数:is_linebuffered
// 功能:检查文件流是否行缓冲
// 参数:
//   - FILE *:文件指针
// 返回值:
//   - int:如果文件流行缓冲返回非零值,否则返回零
int is_linebuffered(FILE *);

// 函数:buffer_size
// 功能:获取文件流的缓冲区大小
// 参数:
//   - FILE *:文件指针
// 返回值:
//   - int:文件流的缓冲区大小
int buffer_size(FILE *);

/**
 * 主函数,程序入口点.
 *
 * 此函数执行以下操作:
 * 1. 提示用户输入任意字符.
 * 2. 读取用户输入的字符,如果遇到文件结束符EOF,则报告错误.
 * 3. 向标准错误输出写入一行文本.
 * 4. 打印stdin,stdout和stderr的状态.
 * 5. 尝试打开/etc/passwd文件,如果失败则报告错误.
 * 6. 读取文件中的第一个字符,如果遇到EOF,则报告错误.
 * 7. 打印/etc/passwd文件的状态.
 * 8. 正常退出程序.
 */
int main(void)
{
    FILE *fp;

    // 提示用户输入任意字符
    fputs("enter any character\n", stdout);
    // 读取用户输入的字符,如果遇到EOF,则报告错误
    if (getchar() == EOF)
        err_sys("getchar error");
    // 向标准错误输出写入一行文本
    fputs("one line to standard error\n", stderr);

    // 打印stdin,stdout和stderr的状态
    pr_stdio("stdin", stdin);
    pr_stdio("stdout", stdout);
    pr_stdio("stderr", stderr);

    // 尝试打开/etc/passwd文件,如果失败则报告错误
    if ((fp = fopen("/etc/passwd", "r")) == NULL)
        err_sys("fopen error");
    // 读取文件中的第一个字符,如果遇到EOF,则报告错误
    if (getc(fp) == EOF)
        err_sys("getc error");
    // 打印/etc/passwd文件的状态
    pr_stdio("/etc/passwd", fp);
    // 正常退出程序
    exit(0);
}

/**
 * @brief 打印标准I/O流的信息
 *
 * @param name 流的名称
 * @param fp 指向FILE结构的指针
 */
void pr_stdio(const char *name, FILE *fp)
{
    // 打印流的名称
    printf("stream = %s, ", name);

    // 判断流是否无缓冲
    if (is_unbuffered(fp))
        printf("unbuffered");
    // 判断流是否行缓冲
    else if (is_linebuffered(fp))
        printf("line buffered");
    // 如果既不是无缓冲也不是行缓冲,则为全缓冲
    else /* if neither of above */
        printf("fully buffered");

    // 打印缓冲区大小
    printf(", buffer size = %d\n", buffer_size(fp));
}

/*
 * 以下代码是非可移植的.
 */

// 判断文件流是否未缓冲
// 参数: fp - 文件流指针
// 返回: 如果文件流未缓冲则返回非零值,否则返回0
#if defined(_IO_UNBUFFERED)
int is_unbuffered(FILE *fp)
{
    return (fp->_flags & _IO_UNBUFFERED);
}

// 判断文件流是否行缓冲
// 参数: fp - 文件流指针
// 返回: 如果文件流行缓冲则返回非零值,否则返回0
int is_linebuffered(FILE *fp)
{
    return (fp->_flags & _IO_LINE_BUF);
}

// 获取文件流的缓冲区大小
// 参数: fp - 文件流指针
// 返回: 缓冲区的大小(字节数)
int buffer_size(FILE *fp)
{
    return (fp->_IO_buf_end - fp->_IO_buf_base);
}

#elif defined(__SNBF)

// 判断文件流是否未缓冲
// 参数: fp - 文件流指针
// 返回: 如果文件流未缓冲则返回非零值,否则返回0
int is_unbuffered(FILE *fp)
{
    return (fp->_flags & __SNBF);
}

// 判断文件流是否行缓冲
// 参数: fp - 文件流指针
// 返回: 如果文件流行缓冲则返回非零值,否则返回0
int is_linebuffered(FILE *fp)
{
    return (fp->_flags & __SLBF);
}

// 获取文件流的缓冲区大小
// 参数: fp - 文件流指针
// 返回: 缓冲区的大小(字节数)
int buffer_size(FILE *fp)
{
    return (fp->_bf._size);
}

#elif defined(_IONBF)

/* # wmtag_memo_我修改了这里_开始 */
/*
#ifdef _LP64
#define _flag __pad[4]
#define _ptr __pad[1]
#define _base __pad[2]
#endif
*/
/* # wmtag_memo_我修改了这里_结束 */

int is_unbuffered(FILE *fp)
{
    /* # wmtag_memo_我修改了这里_开始 */
    // 检查文件指针fp是否设置为无缓冲模式
    // _IONBF标志表示无缓冲模式
    return (fp->_flags & _IONBF);
    /* # wmtag_memo_我修改了这里_结束 */
}

int is_linebuffered(FILE *fp)
{
    /* # wmtag_memo_我修改了这里_开始 */
    // 检查文件指针fp是否设置为行缓冲模式
    // _IOLBF标志表示行缓冲模式,即每输出一个换行符就刷新缓冲区
    return (fp->_flags & _IOLBF);
    /* # wmtag_memo_我修改了这里_结束 */
}

int buffer_size(FILE *fp)
{
    /**
     * 获取文件流的缓冲区大小.
     * 如果定义了_LP64,则返回实际缓冲区的大小,即缓冲区末尾指针与起始指针之差.
     * 否则,返回BUFSIZ,这是标准缓冲区大小的猜测值.
     *
     * @param fp 文件流指针
     * @return 缓冲区大小
     */
#ifdef _LP64
    /* # wmtag_memo_我修改了这里_开始 */
    // 返回实际缓冲区的大小
    return (fp->_IO_buf_end - fp->_IO_buf_base);
    /* # wmtag_memo_我修改了这里_结束 */
#else
    // 返回标准缓冲区大小的猜测值
    return (BUFSIZ); /* just a guess */
#endif
}

#else

#error unknown stdio implementation!

#endif
