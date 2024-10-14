#include "apue.h"
#include <sys/socket.h>

#define MAXSLEEP 128

/**
 * @brief 尝试使用指数退避策略连接到指定的地址
 *
 * @param domain 协议族,如AF_INET
 * @param type 套接字类型,如SOCK_STREAM
 * @param protocol 协议,通常为0以使用默认协议
 * @param addr 指向sockaddr结构体的指针,包含要连接的地址信息
 * @param alen sockaddr结构体的大小
 * @return int 成功时返回套接字描述符,失败时返回-1
 */
int connect_retry(int domain, int type, int protocol, const struct sockaddr *addr, socklen_t alen)
{
    int numsec, fd;

    // 使用指数退避策略尝试连接
    for (numsec = 1; numsec <= MAXSLEEP; numsec <<= 1)
    {
        // 创建套接字
        if ((fd = socket(domain, type, protocol)) < 0)
            return (-1);
        // 尝试连接
        if (connect(fd, addr, alen) == 0)
        {
            // 连接成功
            return (fd);
        }
        // 关闭套接字
        close(fd);

        // 在重试前延迟一段时间
        if (numsec <= MAXSLEEP / 2)
            sleep(numsec);
    }
    // 所有尝试都失败
    return (-1);
}
