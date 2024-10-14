#include "apue.h"        // 引入APUE库的头文件,该库提供了一套用于UNIX和类UNIX系统的应用程序编程接口.
#include <sys/socket.h>  // 引入系统socket库的头文件,用于网络通信的编程接口.

/*
 * 返回一个全双工管道(一个 UNIX 域套接字),
 * 两个文件描述符分别在 fd[0] 和 fd[1] 中返回.
 */
int fd_pipe(int fd[2])
{
    // 创建一对匿名的Unix域套接字,用于进程间通信
    // AF_UNIX: 指定使用Unix域协议
    // SOCK_STREAM: 指定套接字类型为流式套接字,提供可靠的连接服务
    // 0: 使用默认协议
    // fd: 输出参数,用于存储创建的套接字对文件描述符
    return (socketpair(AF_UNIX, SOCK_STREAM, 0, fd));
}
