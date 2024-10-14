#include "apue.h"
#include <sys/socket.h>

/* 控制缓冲区大小,用于发送/接收一个文件描述符 */
#define CONTROLLEN CMSG_LEN(sizeof(int))

/* 动态分配的控制消息头指针,首次使用时分配内存 */
static struct cmsghdr *cmptr = NULL;

/*
 * 向另一个进程传递一个文件描述符.
 * 如果fd<0,则将-fd作为错误状态发送回去.
 *
 * @param fd 当前进程使用的套接字描述符
 * @param fd_to_send 需要传递的文件描述符
 * @return 成功返回0,失败返回-1
 */
int send_fd(int fd, int fd_to_send)
{
    struct iovec  iov[1];  // 用于传递消息的io向量
    struct msghdr msg;     // 消息头结构体
    char          buf[2];  // 发送/接收协议使用的缓冲区,2字节

    iov[0].iov_base = buf;   // 设置io向量的基地址为buf
    iov[0].iov_len  = 2;     // 设置io向量的长度为2字节
    msg.msg_iov     = iov;   // 设置消息的io向量为iov
    msg.msg_iovlen  = 1;     // 设置消息包含一个io向量
    msg.msg_name    = NULL;  // 消息没有地址信息
    msg.msg_namelen = 0;     // 地址信息长度为0

    if (fd_to_send < 0)
    {
        msg.msg_control    = NULL;         // 不包含控制信息
        msg.msg_controllen = 0;            // 控制信息长度为0
        buf[1]             = -fd_to_send;  // 非零状态表示错误
        if (buf[1] == 0)
            buf[1] = 1;  // 防止-256等值破坏协议
    }
    else
    {
        if (cmptr == NULL && (cmptr = malloc(CONTROLLEN)) == NULL)  // 分配控制消息头内存
            return (-1);
        cmptr->cmsg_level        = SOL_SOCKET;  // 设置控制消息级别为SOL_SOCKET
        cmptr->cmsg_type         = SCM_RIGHTS;  // 设置控制消息类型为SCM_RIGHTS
        cmptr->cmsg_len          = CONTROLLEN;  // 设置控制消息长度
        msg.msg_control          = cmptr;       // 设置消息的控制信息为cmptr
        msg.msg_controllen       = CONTROLLEN;  // 设置控制信息长度
        *(int *)CMSG_DATA(cmptr) = fd_to_send;  // 设置要传递的文件描述符
        buf[1]                   = 0;           // 零状态表示成功
    }

    buf[0] = 0;                     // 空字节标志,用于recv_fd()函数
    if (sendmsg(fd, &msg, 0) != 2)  // 发送消息
        return (-1);
    return (0);  // 发送成功
}
