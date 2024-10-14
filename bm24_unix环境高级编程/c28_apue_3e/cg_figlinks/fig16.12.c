#include "apue.h"        // 引入APUE库的头文件,该库提供了一套用于UNIX和类UNIX系统的应用程序编程接口
#include <errno.h>       // 引入errno头文件,用于访问系统调用的错误代码
#include <sys/socket.h>  // 引入sys/socket.h头文件,包含了socket编程所需的函数原型和数据结构

/*
 * 初始化服务器套接字
 * @param type 套接字类型 (SOCK_STREAM 或 SOCK_SEQPACKET)
 * @param addr 套接字地址结构体指针
 * @param alen 地址结构体的长度
 * @param qlen 最大连接队列长度
 * @return 成功返回套接字文件描述符,失败返回-1
 */
int initserver(int type, const struct sockaddr *addr, socklen_t alen, int qlen)
{
    int fd;       // 套接字文件描述符
    int err = 0;  // 错误码

    // 创建套接字
    if ((fd = socket(addr->sa_family, type, 0)) < 0)
        return (-1);

    // 绑定套接字到指定地址
    if (bind(fd, addr, alen) < 0)
        goto errout;

    // 如果是流式套接字或序列包套接字,则开始监听
    // 如果套接字类型是SOCK_STREAM(TCP)或SOCK_SEQPACKET(可靠的UDP),则进行监听
    if (type == SOCK_STREAM || type == SOCK_SEQPACKET)
    {
        // 尝试监听套接字,qlen指定等待连接的最大队列长度
        // 如果监听失败(返回值小于0),则跳转到错误处理标签errout
        if (listen(fd, qlen) < 0)
            goto errout;
    }

    // 返回创建成功的套接字文件描述符
    return (fd);

errout:
    // 发生错误时,保存错误码并关闭套接字
    err = errno;
    close(fd);
    errno = err;
    // 返回错误码
    return (-1);
}
