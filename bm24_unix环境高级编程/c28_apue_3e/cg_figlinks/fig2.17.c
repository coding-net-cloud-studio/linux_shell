#include "apue.h"    // 引入自定义的头文件apue.h,可能包含程序所需的函数声明和宏定义
#include <errno.h>   // 引入标准库头文件errno.h,用于错误处理
#include <limits.h>  // 引入标准库头文件limits.h,用于获取数据类型和算法的极限值

// 如果定义了OPEN_MAX,则使用其值,否则初始化为0
#ifdef OPEN_MAX
// 如果定义了OPEN_MAX,则openmax的值为OPEN_MAX的值
static long openmax = OPEN_MAX;
#else
// 如果没有定义OPEN_MAX,则openmax的值为0
static long openmax = 0;
#endif

// 如果OPEN_MAX不确定,这是一个可能不充分的猜测值
#define OPEN_MAX_GUESS 256

/**
 * open_max - 获取系统中同时打开文件的最大数量
 *
 * 返回值: 成功返回系统中同时打开文件的最大数量,失败返回-1
 */
long open_max(void)
{
    // 如果openmax未初始化
    if (openmax == 0)
    {               /* first time through */
        errno = 0;  // 清除errno错误标志
        // 使用sysconf获取_SC_OPEN_MAX的值
        if ((openmax = sysconf(_SC_OPEN_MAX)) < 0)
        {
            // 如果errno仍为0,说明_SC_OPEN_MAX的值不确定
            if (errno == 0)
                openmax = OPEN_MAX_GUESS; /* it's indeterminate */
            else
                // 否则,报告sysconf错误
                err_sys("sysconf error for _SC_OPEN_MAX");
        }
    }
    return (openmax);  // 返回最大打开文件数
}
