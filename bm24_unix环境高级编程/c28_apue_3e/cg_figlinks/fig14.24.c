#include "apue.h"  // 引入apue库,该库提供了一系列系统编程的工具和函数

/**
 * @brief 向文件描述符写入指定数量的字节.
 *
 * 这个函数会尝试将 `ptr` 指向的内存区域中的 `n` 个字节写入到文件描述符 `fd` 中.
 * 如果写入过程中发生错误,函数会返回已经写入的字节数,或者在严重错误的情况下返回 -1.
 * 如果函数成功写入了所有 `n` 个字节,它将返回写入的字节总数(即 `n`).
 *
 * @param fd 文件描述符,指向要写入的文件或设备.
 * @param ptr 指向要写入数据的缓冲区的指针.
 * @param n 要写入的字节数.
 * @return 如果成功写入所有字节,返回写入的字节总数;如果发生错误,返回 -1 或已经写入的字节数.
 */
ssize_t /* Write "n" bytes to a descriptor  */
writen(int fd, const void *ptr, size_t n)
{
    // nleft 变量用于存储待写入的字节数.
    size_t nleft;

    // nwritten 变量用于存储实际写入的字节数.
    ssize_t nwritten;

    // 计算还剩下多少字节需要写入
    nleft = n;
    // 循环写入,直到所有字节都被写入
    while (nleft > 0)
    {
        // 尝试写入剩余的字节
        if ((nwritten = write(fd, ptr, nleft)) < 0)
        {
            // 如果是第一次写入就出错,返回-1表示错误
            if (nleft == n)
                return (-1);
            // 如果不是第一次写入出错,返回已经写入的字节数
            else
                break;
        }
        else if (nwritten == 0)
        {
            // 如果写入了0字节,可能是文件描述符已经关闭,或者发生了其他错误,退出循环
            break;
        }
        // 更新剩余的字节数和写入的指针位置
        // nleft 减去已写入的字节数
        nleft -= nwritten;
        // 指针向前移动已写入的字节数
        ptr += nwritten;
    }
    // 返回还未写入的字节数(即总字节数减去已经写入的字节数)
    return (n - nleft);
}
