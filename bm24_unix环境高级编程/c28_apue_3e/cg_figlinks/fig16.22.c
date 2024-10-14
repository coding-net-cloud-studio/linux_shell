#include "apue.h"        // 引入APUE库的头文件
#include <errno.h>       // 引入错误处理头文件
#include <sys/socket.h>  // 引入套接字编程所需的头文件

/*
 * 初始化服务器套接字
 * @param type 套接字类型,如 SOCK_STREAM 或 SOCK_DGRAM
 * @param addr 指向包含服务器地址的 sockaddr 结构体的指针
 * @param alen sockaddr 结构体的大小
 * @param qlen 等待连接的最大队列长度
 * @return 成功时返回套接字文件描述符,失败时返回 -1
 */
int initserver(int type, const struct sockaddr *addr, socklen_t alen, int qlen)
{
    int fd, err;    // fd 为套接字文件描述符,err 用于存储错误码
    int reuse = 1;  // 设置 SO_REUSEADDR 选项,允许地址重用

    // 创建套接字
    if ((fd = socket(addr->sa_family, type, 0)) < 0)
        return (-1);

    // 设置套接字选项,允许地址重用
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int)) < 0)
        goto errout;

    // 绑定套接字到指定地址
    if (bind(fd, addr, alen) < 0)
        goto errout;

    // 如果是流式或顺序包套接字,开始监听
    if (type == SOCK_STREAM || type == SOCK_SEQPACKET)
        if (listen(fd, qlen) < 0)
            goto errout;

    // 返回创建成功的套接字文件描述符
    return (fd);

errout:
    // 发生错误时,保存错误码并关闭套接字
    err = errno;
    close(fd);
    errno = err;
    // 返回 -1 表示失败
    return (-1);
}
