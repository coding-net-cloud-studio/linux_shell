#include "apue.h"        // 引入APUE库的头文件
#include <sys/socket.h>  // 引入系统socket库的头文件

#define MAXSLEEP 128  // 定义最大睡眠时间为128

/*
 * connect_retry - 尝试连接服务器,使用指数退避策略
 *
 * @sockfd: 要连接的套接字文件描述符
 * @addr: 包含服务器地址的结构体指针
 * @alen: 地址结构体的长度
 *
 * 返回值: 成功连接返回0,否则返回-1
 */
int connect_retry(int sockfd, const struct sockaddr *addr, socklen_t alen)
{
    int numsec;  // 用于指数退避的秒数

    // 使用指数退避策略尝试连接
    for (numsec = 1; numsec <= MAXSLEEP; numsec <<= 1)
    {
        if (connect(sockfd, addr, alen) == 0)
        {
            // 连接成功
            return (0);
        }

        // 连接失败,等待一段时间后重试
        if (numsec <= MAXSLEEP / 2)
            sleep(numsec);
    }
    // 所有尝试均失败
    return (-1);
}
