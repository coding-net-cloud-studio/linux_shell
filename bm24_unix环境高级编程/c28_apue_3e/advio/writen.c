#include "apue.h"

/**
 * @brief 将 "n" 字节写入描述符
 *
 * @param fd 文件描述符
 * @param ptr 指向要写入数据的指针
 * @param n 要写入的字节数
 * @return ssize_t 成功时返回写入的字节数,失败时返回 -1
 */
ssize_t /* Write "n" bytes to a descriptor  */
writen(int fd, const void *ptr, size_t n)
{
    size_t  nleft;     // 剩余未写的字节数
    ssize_t nwritten;  // 实际写入的字节数

    nleft = n;
    // 循环直到所有字节都被写入
    while (nleft > 0)
    {
        // 尝试写入数据
        if ((nwritten = write(fd, ptr, nleft)) < 0)
        {
            // 如果是第一次尝试写入就失败,返回 -1
            if (nleft == n)
                return (-1);
            else
                break;  // 否则,跳出循环,返回已写入的字节数
        }
        else if (nwritten == 0)
        {
            // 如果没有写入任何数据,跳出循环
            break;
        }
        // 更新剩余字节数和指针位置
        nleft -= nwritten;
        ptr += nwritten;
    }
    // 返回成功写入的总字节数
    return (n - nleft);
}
