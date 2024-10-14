#include "apue.h"
#include <sys/socket.h> /* struct msghdr */

/* 定义控制缓冲区的大小,用于发送/接收一个文件描述符 */
#define CONTROLLEN CMSG_LEN(sizeof(int))

/* cmptr 是一个指向 cmsghdr 结构的指针,该结构用于在套接字上发送和接收文件描述符.
 * 它将在第一次使用时动态分配内存.
 */
static struct cmsghdr *cmptr = NULL;
/*
 * 从服务器进程接收文件描述符.同时,接收到的任何数据都会传递给 (*userfunc)(STDERR_FILENO, buf, nbytes).
 * 我们有一个2字节的协议用于从send_fd()接收fd.
 */
int recv_fd(int fd, ssize_t (*userfunc)(int, const void *, size_t))
{
    int           newfd, nr, status;  // newfd: 新的文件描述符, nr: 接收到的字节数, status: 状态字节
    char         *ptr;                // ptr: 指向缓冲区的指针
    char          buf[MAXLINE];       // buf: 存储接收到的数据的缓冲区
    struct iovec  iov[1];             // iov: I/O向量,用于recvmsg
    struct msghdr msg;                // msg: 消息头结构,用于recvmsg

    status = -1;  // 初始化状态为-1
    for (;;)      // 无限循环直到接收到有效的文件描述符或连接关闭
    {
        iov[0].iov_base = buf;                                      // 设置I/O向量的基地址为缓冲区
        iov[0].iov_len  = sizeof(buf);                              // 设置I/O向量的长度为缓冲区大小
        msg.msg_iov     = iov;                                      // 设置消息头的I/O向量为当前I/O向量
        msg.msg_iovlen  = 1;                                        // 设置消息头的I/O向量数量为1
        msg.msg_name    = NULL;                                     // 消息头名称为空
        msg.msg_namelen = 0;                                        // 消息头名称长度为0
        if (cmptr == NULL && (cmptr = malloc(CONTROLLEN)) == NULL)  // 分配控制缓冲区
            return (-1);                                            // 如果分配失败,返回-1
        msg.msg_control    = cmptr;                                 // 设置消息头的控制信息为分配的控制缓冲区
        msg.msg_controllen = CONTROLLEN;                            // 设置消息头的控制信息长度为CONTROLLEN
        if ((nr = recvmsg(fd, &msg, 0)) < 0)                        // 接收消息
        {
            err_ret("recvmsg error");  // 如果接收失败,打印错误并返回-1
            return (-1);
        }
        else if (nr == 0)  // 如果接收到的字节数为0,表示连接关闭
        {
            err_ret("connection closed by server");  // 打印错误信息
            return (-1);                             // 返回-1
        }

        // 检查是否是带有null和状态的最后数据
        for (ptr = buf; ptr < &buf[nr];)
        {
            if (*ptr++ == 0)  // 如果找到null字符
            {
                if (ptr != &buf[nr - 1])               // 检查null字符是否在倒数第二个位置
                    err_dump("message format error");  // 如果不是,打印格式错误
                status = *ptr & 0xFF;                  // 获取状态字节,并防止符号扩展
                if (status == 0)                       // 如果状态字节为0,表示有文件描述符要接收
                {
                    if (msg.msg_controllen != CONTROLLEN)  // 检查控制信息长度是否正确
                        err_dump("status = 0 but no fd");  // 如果不正确,打印错误
                    newfd = *(int *)CMSG_DATA(cmptr);      // 从控制信息中获取文件描述符
                }
                else  // 如果状态字节不为0,表示是一个错误码
                {
                    newfd = -status;  // 将错误码转换为负数作为文件描述符
                }
                nr -= 2;  // 减去null字符和状态字节
            }
        }
        if (nr > 0 && (*userfunc)(STDERR_FILENO, buf, nr) != nr)  // 如果还有数据,调用userfunc处理
            return (-1);                                          // 如果处理失败,返回-1
        if (status >= 0)                                          // 如果状态字节大于等于0,表示最后的数据已经到达
            return (newfd);                                       // 返回文件描述符或错误码
    }
}
