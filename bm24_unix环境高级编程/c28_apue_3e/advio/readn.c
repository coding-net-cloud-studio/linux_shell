#include "apue.h"

/**
 * @brief 从描述符fd中读取n个字节到ptr指向的内存
 * @param fd 文件描述符
 * @param ptr 指向要读取数据的内存地址
 * @param n 要读取的字节数
 * @return 成功时返回实际读取的字节数,失败时返回-1
 */
ssize_t /* Read "n" bytes from a descriptor  */
readn(int fd, void *ptr, size_t n)
{
    size_t  nleft;  // 剩余未读取的字节数
    ssize_t nread;  // 实际读取的字节数

    nleft = n;
    while (nleft > 0)  // 当还有字节需要读取时循环
    {
        if ((nread = read(fd, ptr, nleft)) < 0)  // 读取失败
        {
            if (nleft == n)  // 如果是第一次尝试读取就失败,则返回-1
                return (-1);
            else  // 否则,已经读取了一部分数据,退出循环
                break;
        }
        else if (nread == 0)  // 读取到文件末尾
        {
            break;  // 退出循环
        }
        nleft -= nread;  // 更新剩余未读取的字节数
        ptr += nread;    // 更新指向下一个要读取的内存地址
    }
    return (n - nleft);  // 返回实际读取的字节数
}
