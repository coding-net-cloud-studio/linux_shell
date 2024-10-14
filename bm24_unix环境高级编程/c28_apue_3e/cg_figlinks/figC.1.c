#include "apue.h"          // 引入APUE库的头文件
#include <limits.h>        // 引入定义各种数据类型限制的头文件
#include <sys/resource.h>  // 引入定义系统资源的头文件

// 定义一个宏,用于猜测可以打开的最大文件数
#define OPEN_MAX_GUESS 256

/**
 * @brief 获取系统中可以打开的最大文件数
 *
 * 该函数尝试使用sysconf获取系统限制打开的最大文件数,如果失败或返回LONG_MAX,
 * 则尝试使用getrlimit获取当前进程的限制.如果rlim_max为RLIM_INFINITY,则使用一个猜测值.
 *
 * @return 返回可以打开的最大文件数
 */
long open_max(void)
{
    long          openmax;  // 存储最大文件数
    struct rlimit rl;       // 用于存储文件限制的结构体

    // 尝试使用sysconf获取系统限制打开的最大文件数
    // 获取系统中可以打开的最大文件数
    // 如果获取失败或者达到LONG_MAX,则表示无法确定最大文件数
    if ((openmax = sysconf(_SC_OPEN_MAX)) < 0 ||
        openmax == LONG_MAX)
    {
        // 如果sysconf失败,尝试使用getrlimit获取当前进程的限制
        if (getrlimit(RLIMIT_NOFILE, &rl) < 0)
            err_sys("can't get file limit");  // 获取限制失败时错误处理
        // 如果rlim_max为RLIM_INFINITY,使用一个猜测值
        // 如果rl.rlim_max等于RLIM_INFINITY,则将openmax设置为OPEN_MAX_GUESS的猜测值
        if (rl.rlim_max == RLIM_INFINITY)
            openmax = OPEN_MAX_GUESS;
        else
            // 否则,使用rl.rlim_max的实际值作为openmax
            openmax = rl.rlim_max;
    }
    return (openmax);  // 返回最大文件数
}
