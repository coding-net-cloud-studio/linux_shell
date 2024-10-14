#include "apue.h"
#include <errno.h>
#include <sys/socket.h>

// 初始化服务器套接字
// type: 套接字类型,如 SOCK_STREAM 或 SOCK_DGRAM
// addr: 服务器地址结构体指针
// alen: 地址结构体的长度
// qlen: 请求队列的最大长度
// 返回值: 成功返回套接字文件描述符,失败返回 -1
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
    if (type == SOCK_STREAM || type == SOCK_SEQPACKET)
    {
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
    // 返回错误标识
    return (-1);
}
