#include "apue.h"        // 引入apue库的头文件
#include <sys/socket.h>  // 引入socket系统的头文件,包含了struct msghdr结构体定义

/* 控制缓冲区的大小,用于发送/接收一个文件描述符 */
// 定义CONTROLLEN为CMSG_LEN宏的结果,该宏计算传递sizeof(int)所需的空间大小
#define CONTROLLEN CMSG_LEN(sizeof(int))

/* cmptr是一个指向cmsghdr结构体的指针,用于存储控制消息头信息
   它在第一次使用时通过malloc分配内存 */
static struct cmsghdr *cmptr = NULL;  // 初始化为NULL,表示还未分配内存

/*
 * Receive a file descriptor from a server process.  Also, any data
 * received is passed to (*userfunc)(STDERR_FILENO, buf, nbytes).
 * We have a 2-byte protocol for receiving the fd from send_fd().
 */

/*
 * 从服务器进程接收一个文件描述符.同时,接收到的任何数据都会传递给 (*userfunc)(STDERR_FILENO, buf, nbytes).
 * 我们有一个2字节的协议用于从send_fd()接收fd.
 */

int recv_fd(int fd, ssize_t (*userfunc)(int, const void *, size_t))
{
    int           newfd, nr, status;  // newfd: 新的文件描述符, nr: 接收到的字节数, status: 状态字节
    char         *ptr;                // 指向buf的指针
    char          buf[MAXLINE];       // 缓冲区用于接收数据
    struct iovec  iov[1];             // iovec结构体用于recvmsg
    struct msghdr msg;                // msghdr结构体用于recvmsg

    status = -1;  // 初始化状态为-1
    for (;;)
    {
        iov[0].iov_base = buf;                                      // 设置接收数据的起始地址
        iov[0].iov_len  = sizeof(buf);                              // 设置接收数据的长度
        msg.msg_iov     = iov;                                      // 设置msg_iov指向iov
        msg.msg_iovlen  = 1;                                        // 设置msg_iovlen为1
        msg.msg_name    = NULL;                                     // 不使用msg_name
        msg.msg_namelen = 0;                                        // msg_namelen为0
        if (cmptr == NULL && (cmptr = malloc(CONTROLLEN)) == NULL)  // 分配内存给cmptr
            return (-1);                                            // 内存分配失败,返回-1
        msg.msg_control    = cmptr;                                 // 设置msg_control指向cmptr
        msg.msg_controllen = CONTROLLEN;                            // 设置msg_controllen为CONTROLLEN
        if ((nr = recvmsg(fd, &msg, 0)) < 0)                        // 接收消息
        {
            err_ret("recvmsg error");  // 接收消息出错
            return (-1);               // 返回-1
        }
        else if (nr == 0)  // 连接被服务器关闭
        {
            err_ret("connection closed by server");  // 打印错误信息
            return (-1);                             // 返回-1
        }

        /*
         * See if this is the final data with null & status.  Null
         * is next to last byte of buffer; status byte is last byte.
         * Zero status means there is a file descriptor to receive.
         */

        /*
         * 检查这是否是带有空值和状态的最后数据.空值
         * 是缓冲区的倒数第二个字节;状态字节是最后一个字节.
         * 状态为零意味着有一个文件描述符接收.
         */

        // 检查是否是带有null和状态的最后数据
        for (ptr = buf; ptr < &buf[nr];)
        {
            if (*ptr++ == 0)  // 找到null字符
            {
                if (ptr != &buf[nr - 1])
                    err_dump("message format error");  // 消息格式错误
                status = *ptr & 0xFF;                  // 获取状态字节并防止符号扩展
                if (status == 0)                       // 如果状态为0,表示有文件描述符要接收
                {
                    if (msg.msg_controllen != CONTROLLEN)
                        err_dump("status = 0 but no fd");  // 状态为0但没有文件描述符
                    newfd = *(int *)CMSG_DATA(cmptr);      // 从控制信息中获取新的文件描述符
                }
                else  // 状态不为0,表示错误码
                {
                    newfd = -status;  // 将错误码转换为负数
                }
                nr -= 2;  // 减去null和状态字节
            }
        }
        if (nr > 0 && (*userfunc)(STDERR_FILENO, buf, nr) != nr)  // 如果还有数据,调用userfunc处理
            return (-1);                                          // userfunc处理失败,返回-1
        if (status >= 0)                                          // 最终数据已到达
            return (newfd);                                       // 返回新的文件描述符或错误码
    }
}
