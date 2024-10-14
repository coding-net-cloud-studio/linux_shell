#include "apue.h"
#include <errno.h>
#include <sys/socket.h>

/**
 * @brief 初始化服务器套接字
 *
 * @param type 套接字类型,如 SOCK_STREAM 或 SOCK_DGRAM
 * @param addr 服务器地址结构体指针
 * @param alen 地址结构体的长度
 * @param qlen 请求队列的最大长度
 * @return int 成功返回套接字文件描述符,失败返回 -1
 */
int initserver(int type, const struct sockaddr *addr, socklen_t alen, int qlen)
{
    // 定义文件描述符fd和错误码err,用于后续的文件操作和错误处理
    int fd, err;
    // 定义reuse变量并初始化为1,用于设置socket选项,允许地址重用
    int reuse = 1;

    // 创建套接字
    // 创建一个套接字
    // 参数:
    //   addr->sa_family: 地址族(AF_INET, AF_INET6等)
    //   type: 套接字类型(SOCK_STREAM, SOCK_DGRAM等)
    //   0: 使用默认协议
    // 返回值:
    //   成功: 套接字文件描述符
    //   失败: -1
    if ((fd = socket(addr->sa_family, type, 0)) < 0)
        return (-1);  // 如果创建套接字失败,返回-1

    // 设置套接字选项,允许地址重用
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int)) < 0)
        goto errout;

    // 绑定套接字到指定地址
    if (bind(fd, addr, alen) < 0)
        goto errout;

    // 如果是流式套接字或序列包套接字,则开始监听
    // 如果套接字类型是SOCK_STREAM(TCP)或SOCK_SEQPACKET(可靠的UDP),则尝试监听该套接字
    if (type == SOCK_STREAM || type == SOCK_SEQPACKET)
        // 调用listen函数开始监听指定的套接字,qlen参数指定了等待连接的最大队列长度
        // 如果监听失败(返回值小于0),则跳转到errout标签处理错误
        if (listen(fd, qlen) < 0)
            goto errout;

    // 返回套接字文件描述符
    return (fd);

errout:
    // 发生错误时,保存错误号并关闭套接字
    // 保存当前的errno值,因为close函数可能会改变它
    err = errno;
    // 关闭文件描述符fd
    close(fd);
    // 恢复之前保存的errno值
    errno = err;
    // 返回-1表示操作失败
    return (-1);
}
