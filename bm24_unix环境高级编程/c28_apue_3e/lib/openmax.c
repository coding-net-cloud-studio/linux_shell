#include "apue.h"    // 引入APUE库的头文件
#include <errno.h>   // 引入错误处理头文件
#include <limits.h>  // 引入定义各种数据类型限制的头文件

// 以下代码段没有具体的函数方法,因此无需添加标准文档注释.
// 如果后续代码中有具体的函数方法,请为每个函数添加相应的文档注释.

// 如果定义了OPEN_MAX,则使用其值,否则初始化为0
// 定义一个静态长整型变量openmax,用于存储系统允许打开的最大文件数
#ifdef OPEN_MAX                  // 如果定义了OPEN_MAX宏
static long openmax = OPEN_MAX;  // 则将openmax初始化为OPEN_MAX的值
#else                            // 如果没有定义OPEN_MAX宏
static long openmax = 0;  // 则将openmax初始化为0
#endif

// 如果OPEN_MAX不确定,使用这个猜测值
#define OPEN_MAX_GUESS 256

/**
 * 获取系统中可以打开的最大文件数
 *
 * @return 返回系统中可以打开的最大文件数,如果无法确定则返回猜测值OPEN_MAX_GUESS
 */
long open_max(void)
{
    // 如果openmax还未被初始化
    if (openmax == 0)
    {               /* first time through */
        errno = 0;  // 清除errno错误码
        // 使用sysconf获取_SC_OPEN_MAX的值
        if ((openmax = sysconf(_SC_OPEN_MAX)) < 0)
        {
            // 如果errno没有被设置,说明_SC_OPEN_MAX的值是不确定的
            if (errno == 0)
                openmax = OPEN_MAX_GUESS;  // 使用猜测值
            else
                err_sys("sysconf error for _SC_OPEN_MAX");  // 否则报告错误
        }
    }
    return (openmax);  // 返回最大文件数
}
