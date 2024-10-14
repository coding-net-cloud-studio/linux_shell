#include "apue.h"    // 引入自定义的apue库,可能包含一些常用的系统编程辅助函数
#include <errno.h>   // 引入标准错误处理头文件,定义了errno变量和错误码
#include <limits.h>  // 引入标准库中的极限值定义,如INT_MAX, CHAR_BIT等

// 如果定义了PATH_MAX,则pathmax的初始值为PATH_MAX
#ifdef PATH_MAX
static long pathmax = PATH_MAX;
// 如果没有定义PATH_MAX,则pathmax的初始值为0
#else
static long pathmax = 0;
#endif

// 定义posix_version和xsi_version变量,用于存储POSIX版本信息
static long posix_version = 0;
static long xsi_version   = 0;

/*
 * 如果PATH_MAX的值不确定,不能保证这个猜测足够大
 * 这里定义了一个宏PATH_MAX_GUESS,其值为1024,作为PATH_MAX的备选值
 */
#define PATH_MAX_GUESS 1024

/**
 * @brief 分配一个足够大的缓冲区来存储路径名,并返回指向该缓冲区的指针.
 *
 * @param sizep 指向存储分配的缓冲区大小的变量的指针.如果不需要知道大小,可以设置为 NULL.
 * @return 返回指向分配的缓冲区的指针,如果分配失败,返回 NULL.
 */
char *
path_alloc(size_t *sizep) /* also return allocated size, if nonnull */
{
    char  *ptr;
    size_t size;

    // 检查 POSIX 版本
    if (posix_version == 0)
        posix_version = sysconf(_SC_VERSION);

    // 检查 XSI 版本
    if (xsi_version == 0)
        xsi_version = sysconf(_SC_XOPEN_VERSION);

    // 第一次调用时,获取 PATH_MAX 的值
    if (pathmax == 0)
    {
        errno = 0;
        // 获取 PATH_MAX 的值,如果无法确定,返回错误
        if ((pathmax = pathconf("/", _PC_PATH_MAX)) < 0)
        {
            // 如果 errno 为 0,表示 PATH_MAX 是不确定的
            if (errno == 0)
                pathmax = PATH_MAX_GUESS;
            // 如果获取失败,记录错误并退出
            else
                err_sys("pathconf error for _PC_PATH_MAX");
        }
        else
        {
            // 增加一个字节以确保包含终止空字符
            pathmax++;
        }
    }

    // 根据 POSIX 和 XSI 版本确定缓冲区大小
    if ((posix_version < 200112L) && (xsi_version < 4))
        size = pathmax + 1;
    else
        size = pathmax;

    // 分配内存
    if ((ptr = malloc(size)) == NULL)
        err_sys("malloc error for pathname");

    // 如果需要,返回分配的缓冲区大小
    if (sizep != NULL)
        *sizep = size;
    return (ptr);
}
