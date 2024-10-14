#include "apue.h"   // 包含APUE库的头文件,提供系统编程的API
#include <fcntl.h>  // 包含文件控制操作的头文件,如open, close等

/**
 * 设置文件描述符的 close-on-exec 标志.
 *
 * 这个函数用于设置文件描述符的 close-on-exec 标志,
 * 当进程执行 exec 系列函数时,带有该标志的文件描述符会被自动关闭.
 *
 * @param fd 文件描述符.
 * @return 成功时返回 0,失败时返回 -1,并设置 errno.
 */
int set_cloexec(int fd)
{
    int val;

    // 获取文件描述符的当前标志
    if ((val = fcntl(fd, F_GETFD, 0)) < 0)
        return (-1);

    // 启用 close-on-exec 标志
    val |= FD_CLOEXEC;

    // 设置文件描述符的新标志
    return (fcntl(fd, F_SETFD, val));
}
