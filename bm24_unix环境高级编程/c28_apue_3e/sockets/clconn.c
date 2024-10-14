#include "apue.h"
#include <sys/socket.h>

#define MAXSLEEP 128

/**
 * @brief 尝试连接到一个套接字地址,并使用指数退避策略重试
 *
 * @param sockfd 要连接的套接字文件描述符
 * @param addr 指向要连接的套接字地址结构的指针
 * @param alen 套接字地址结构的长度
 * @return int 如果连接成功返回0,否则返回-1
 */
int connect_retry(int sockfd, const struct sockaddr *addr, socklen_t alen)
{
    int numsec;  // 用于指数退避的秒数

    // 使用指数退避策略尝试连接
    for (numsec = 1; numsec <= MAXSLEEP; numsec <<= 1)
    {
        // 尝试建立与服务器的连接
        // 参数:
        //   sockfd: 套接字文件描述符
        //   addr: 服务器地址结构体指针
        //   alen: 地址结构体的长度
        // 返回值:
        //   成功返回0,失败返回-1
        if (connect(sockfd, addr, alen) == 0)
        {
            // 连接成功
            return (0);
        }

        // 在重试之前延迟一段时间
        // 如果 numsec 小于或等于 MAXSLEEP 的一半,则休眠 numsec 秒
        // 这样可以避免不必要的长时间休眠,提高程序响应速度
        if (numsec <= MAXSLEEP / 2)
            sleep(numsec);  // 休眠函数,参数为休眠的秒数
    }
    // 所有尝试都失败了
    return (-1);
}
