#include "apue.h"        // 引入APUE库,提供系统编程接口
#include <errno.h>       // 引入错误处理头文件,定义了errno变量和错误码
#include <fcntl.h>       // 引入文件控制操作头文件,如open, fcntl等
#include <sys/socket.h>  // 引入套接字编程所需的头文件
#include <sys/ioctl.h>   // 引入I/O控制操作头文件,用于设备特定的I/O控制
#if defined(BSD) || defined(MACOS) || defined(SOLARIS)
#include <sys/filio.h>  // 若定义了BSD, MACOS或SOLARIS,则引入此头文件,它提供了额外的文件I/O控制功能
#endif

/**
 * @brief 设置套接字为异步模式
 *
 * 该函数尝试将给定的套接字设置为异步模式,以便套接字上的I/O事件可以生成信号.
 *
 * @param sockfd 要设置为异步模式的套接字文件描述符
 * @return int 成功返回0,失败返回-1
 */
int setasync(int sockfd)
{
    // 声明一个整型变量n,用于存储整数值.
    int n;

    // 设置套接字的所有者为本进程ID
    // 使用fcntl函数设置套接字的拥有者进程ID
    // F_SETOWN: 设置套接字的拥有者进程ID
    // sockfd: 套接字描述符
    // getpid(): 获取当前进程的ID
    // 如果设置失败,返回-1
    if (fcntl(sockfd, F_SETOWN, getpid()) < 0)
        return (-1);

    // 将n设置为1,表示启用异步模式
    n = 1;

    // 使用ioctl函数启用套接字的异步I/O

    // 使用ioctl函数设置套接字的异步状态
    // sockfd: 套接字文件描述符
    // FIOASYNC: ioctl请求码,用于设置套接字的异步状态
    // &n: 指向整数的指针,用于接收或设置异步状态
    // 返回值: 如果成功,返回0;如果失败,返回-1
    if (ioctl(sockfd, FIOASYNC, &n) < 0)
        return (-1);  // 如果ioctl调用失败,返回-1

    // 成功设置异步模式
    return (0);
}

/**
 * @brief 清除套接字的异步状态
 *
 * 该函数用于清除指定套接字的异步状态.通过调用ioctl系统调用并传递FIOASYNC命令,
 * 可以将套接字的异步标志设置为0,从而禁用异步通知.
 *
 * @param sockfd 需要操作的套接字文件描述符
 * @return int 如果成功清除异步状态返回0,失败则返回-1
 */
int clrasync(int sockfd)
{
    // 声明一个整型变量n,用于存储整数值.
    int n;

    // 初始化n为0,表示要清除异步状态
    n = 0;
    // 调用ioctl系统调用,尝试清除套接字的异步状态
    if (ioctl(sockfd, FIOASYNC, &n) < 0)
        // 如果ioctl调用失败,返回-1
        return (-1);
    // 如果ioctl调用成功,返回0
    return (0);
}
