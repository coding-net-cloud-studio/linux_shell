#include "apue.h"
#include <sys/socket.h>

/*
 * 创建一个全双工管道(UNIX域套接字),并将两个文件描述符返回到fd[0]和fd[1]中.
 *
 * @param fd 一个包含两个整数的数组,用于存储创建的套接字的文件描述符.
 * @return 成功时返回0,失败时返回-1,并设置errno以指示错误.
 */
int fd_pipe(int fd[2])
{
    // 使用socketpair函数创建一个UNIX域套接字,类型为SOCK_STREAM,表示全双工通信
    return (socketpair(AF_UNIX, SOCK_STREAM, 0, fd));
}
