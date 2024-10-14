#include "apue.h"        // 引入APUE库的头文件
#include <errno.h>       // 引入错误处理头文件
#include <fcntl.h>       // 引入文件控制头文件
#include <sys/socket.h>  // 引入套接字编程所需的头文件
#include <sys/ioctl.h>   // 引入I/O控制头文件
#if defined(BSD) || defined(MACOS) || defined(SOLARIS)
#include <sys/filio.h>  // 若在BSD,MACOS或SOLARIS系统上编译,引入此头文件
#endif

// setasync 函数用于设置套接字的异步模式
// 参数 sockfd 是需要设置的套接字文件描述符
// 返回值 0 表示成功,-1 表示失败
int setasync(int sockfd)
{
    int n;

    // 设置套接字的所有者为本进程的进程ID
    // 如果失败,返回 -1
    if (fcntl(sockfd, F_SETOWN, getpid()) < 0)
        return (-1);

    // 设置异步标志为1,表示启用异步模式
    n = 1;

    // 使用 ioctl 函数设置套接字的异步模式
    // 如果失败,返回 -1
    if (ioctl(sockfd, FIOASYNC, &n) < 0)
        return (-1);

    // 成功设置异步模式,返回 0
    return (0);
}

// clrasync函数用于清除套接字的异步状态
// 参数:
//   sockfd: 需要操作的套接字文件描述符
// 返回值:
//   成功返回0,失败返回-1
int clrasync(int sockfd)
{
    int n;

    // 初始化n为0,表示希望清除异步状态
    n = 0;
    // 使用ioctl系统调用清除套接字的异步状态
    // 如果ioctl调用失败,返回-1
    if (ioctl(sockfd, FIOASYNC, &n) < 0)
        return (-1);
    // 如果ioctl调用成功,返回0
    return (0);
}
