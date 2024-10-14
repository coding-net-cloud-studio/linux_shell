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
    size_t  nleft;     // 剩余未写入的字节数
    ssize_t nwritten;  // 实际写入的字节数

    nleft = n;
    while (nleft > 0)  // 循环直到所有字节都被写入
    {
        if ((nwritten = write(fd, ptr, nleft)) < 0)  // 尝试写入数据
        {
            if (nleft == n)  // 如果是第一次尝试写入就失败,则返回 -1
                return (-1);
            else  // 否则,跳出循环,返回已写入的字节数
                break;
        }
        else if (nwritten == 0)  // 如果没有字节被写入,跳出循环
        {
            break;
        }
        nleft -= nwritten;  // 更新剩余未写入的字节数
        ptr += nwritten;    // 更新指针位置
    }
    return (n - nleft);  // 返回成功写入的字节数
}
