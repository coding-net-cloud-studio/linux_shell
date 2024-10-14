#include "apue.h"        // 引入APUE库的头文件,提供了一系列系统编程的工具和函数
#include <sys/socket.h>  // 引入系统socket库的头文件,用于网络通信

/* 定义控制缓冲区的大小,用于发送/接收一个文件描述符 */
#define CONTROLLEN CMSG_LEN(sizeof(int))

/* 指向cmsghdr结构体的指针,首次使用时动态分配内存 */
static struct cmsghdr *cmptr = NULL;

/**
 * @brief 发送文件描述符到另一个进程.
 *
 * @param fd 本地进程的文件描述符.
 * @param fd_to_send 要发送的文件描述符.
 * @return 如果成功发送文件描述符,返回0;如果发送失败,返回-1.
 */
int send_fd(int fd, int fd_to_send)
{
    // 定义一个iovec结构体数组,用于存储要发送或接收的数据片段
    struct iovec iov[1];

    // 定义一个msghdr结构体,用于存储消息的详细信息
    struct msghdr msg;

    // 定义一个字符数组buf,用于send_fd()/recv_fd()函数的2字节协议
    char buf[2];

    // 设置iov数组的第一个元素的基地址为buf,长度为2
    iov[0].iov_base = buf;
    iov[0].iov_len  = 2;

    // 将iov数组的地址赋给msg的msg_iov成员,表示消息的数据部分
    msg.msg_iov = iov;
    // 设置msg_iov数组的长度为1,因为我们只有一个数据片段
    msg.msg_iovlen = 1;
    // msg_name和msg_namelen用于存储消息的目标地址信息,在这个例子中不需要,所以设置为NULL和0
    msg.msg_name    = NULL;
    msg.msg_namelen = 0;

    // 如果要发送的文件描述符小于0,则发送错误状态
    // 如果fd_to_send小于0,表示发生了错误
    if (fd_to_send < 0)
    {
        // 设置消息控制信息为空
        msg.msg_control = NULL;
        // 消息控制长度设置为0
        msg.msg_controllen = 0;
        // 将错误状态码存入buf的第二个字节
        buf[1] = -fd_to_send;
        // 确保错误状态不为0,防止协议混乱
        if (buf[1] == 0)
            buf[1] = 1;
    }
    else
    {
        // 如果是第一次调用,分配控制消息缓冲区
        if (cmptr == NULL && (cmptr = malloc(CONTROLLEN)) == NULL)
            return (-1);  // 分配失败返回-1
        // 设置控制消息的级别为SOL_SOCKET
        cmptr->cmsg_level = SOL_SOCKET;
        // 设置控制消息的类型为SCM_RIGHTS
        cmptr->cmsg_type = SCM_RIGHTS;
        // 设置控制消息的长度
        cmptr->cmsg_len = CONTROLLEN;
        // 将控制消息缓冲区指针赋给msg结构体
        msg.msg_control = cmptr;
        // 设置消息控制长度
        msg.msg_controllen = CONTROLLEN;
        // 将文件描述符存储在控制消息的数据部分
        *(int *)CMSG_DATA(cmptr) = fd_to_send;
        // 设置状态码为0,表示操作成功
        buf[1] = 0;
    }

    // 设置协议的标志字节
    buf[0] = 0;  // 初始化缓冲区的第一个字节为0
    // 发送消息,如果发送失败则返回-1
    // sendmsg函数用于发送消息,参数fd是文件描述符,&msg是消息结构体的地址,0表示没有特定的标志
    if (sendmsg(fd, &msg, 0) != 2)  // 如果发送的消息不是预期的2个字节,则认为发送失败
        return (-1);                // 发送失败,返回-1
    return (0);                     // 发送成功,返回0
}
