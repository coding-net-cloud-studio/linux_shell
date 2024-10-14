#include "apue.h"
#include <limits.h>
#include <sys/resource.h>

// 定义一个猜测的最大打开文件数
#define OPEN_MAX_GUESS 256

/**
 * 获取系统允许的最大打开文件数
 *
 * @return long 返回系统允许的最大打开文件数,如果出错则返回-1
 */
long open_max(void)
{
    long          openmax;  // 存储最大打开文件数
    struct rlimit rl;       // 存储文件限制信息

    // 尝试使用sysconf获取系统最大打开文件数
    if ((openmax = sysconf(_SC_OPEN_MAX)) < 0 ||
        openmax == LONG_MAX)
    {
        // 如果sysconf失败或返回LONG_MAX,则尝试使用getrlimit获取
        if (getrlimit(RLIMIT_NOFILE, &rl) < 0)
            err_sys("can't get file limit");  // 获取失败则输出错误信息并退出
        // 如果rlim_max为无穷大,则使用预设的猜测值
        // 如果rl.rlim_max等于RLIM_INFINITY,则将openmax设置为OPEN_MAX_GUESS
        if (rl.rlim_max == RLIM_INFINITY)
            openmax = OPEN_MAX_GUESS;
        else
            // 否则,使用rl.rlim_max的值作为openmax的值
            openmax = rl.rlim_max;
    }
    return (openmax);  // 返回最大打开文件数
}
