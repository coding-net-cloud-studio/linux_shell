#include "apue.h"   // 引入APUE库的头文件,该库提供了一套用于编写可移植UNIX应用程序的函数.
#include <fcntl.h>  // 引入文件控制操作的头文件,如open(),fcntl()等函数.

// @brief 设置文件描述符的close-on-exec标志
//
// 该函数通过fcntl系统调用设置给定文件描述符的close-on-exec标志.
// 当一个进程执行exec系列函数时,所有设置了此标志的文件描述符会被自动关闭.
//
// @param fd 需要设置标志的文件描述符
// @return 成功返回0,失败返回-1并设置errno
int set_cloexec(int fd)
{
    int val;

    // 获取当前文件描述符的标志
    // 获取文件描述符的标志
    // 参数:
    //   fd: 文件描述符
    //   cmd: F_GETFD,用于获取文件描述符的标志
    //   arg: 通常设置为0
    // 返回值:
    //   成功返回文件描述符的标志,失败返回-1
    if ((val = fcntl(fd, F_GETFD, 0)) < 0)
        return (-1);

    val |= FD_CLOEXEC;  // 启用close-on-exec标志

    // 设置文件描述符的新标志
    return (fcntl(fd, F_SETFD, val));
}
