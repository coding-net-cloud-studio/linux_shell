#include "apue.h"        // 引入APUE库的头文件
#include <sys/socket.h>  // 引入系统socket库的头文件

#define MAXSLEEP 128  // 定义最大睡眠时间为128

/*
 * connect_retry - 尝试连接服务器,使用指数退避策略
 *
 * @param domain: 套接字使用的地址族
 * @param type: 套接字类型(如 SOCK_STREAM)
 * @param protocol: 使用的协议
 * @param addr: 指向要连接的套接字地址结构的指针
 * @param alen: 地址结构的长度
 *
 * @return: 成功时返回文件描述符,失败时返回 -1
 */
int connect_retry(int domain, int type, int protocol, const struct sockaddr *addr, socklen_t alen)
{
    int numsec, fd;  // numsec 是当前尝试的延迟时间,fd 是套接字文件描述符

    /*
     * 尝试使用指数退避策略连接.
     */
    for (numsec = 1; numsec <= MAXSLEEP; numsec <<= 1)  // 指数增加延迟时间,直到达到最大值
    {
        if ((fd = socket(domain, type, protocol)) < 0)  // 创建套接字
            return (-1);                                // 创建失败,返回 -1
        if (connect(fd, addr, alen) == 0)               // 尝试连接
        {
            /*
             * 连接被接受.
             */
            return (fd);  // 连接成功,返回文件描述符
        }
        close(fd);  // 关闭套接字

        /*
         * 在重试前延迟一段时间.
         */
        if (numsec <= MAXSLEEP / 2)  // 如果延迟时间小于最大值的一半,则睡眠相应的时间
            sleep(numsec);
    }
    return (-1);  // 所有尝试都失败,返回 -1
}
