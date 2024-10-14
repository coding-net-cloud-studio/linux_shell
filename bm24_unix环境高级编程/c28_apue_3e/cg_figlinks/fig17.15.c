#include "apue.h"
#include <sys/socket.h>

// 定义CREDSTRUCT和SCM_CREDTYPE宏,根据不同的操作系统接口
#if defined(SCM_CREDS)                /* BSD接口 */
#define CREDSTRUCT   cmsgcred         // BSD系统使用cmsgcred结构体
#define SCM_CREDTYPE SCM_CREDS        // BSD系统使用SCM_CREDS类型
#elif defined(SCM_CREDENTIALS)        /* Linux接口 */
#define CREDSTRUCT   ucred            // Linux系统使用ucred结构体
#define SCM_CREDTYPE SCM_CREDENTIALS  // Linux系统使用SCM_CREDENTIALS类型
#else
#error passing credentials is unsupported! // 如果都不支持,编译时报错
#endif

// 定义控制缓冲区的大小,用于发送/接收一个文件描述符
#define RIGHTSLEN  CMSG_LEN(sizeof(int))                // 权利部分的长度
#define CREDSLEN   CMSG_LEN(sizeof(struct CREDSTRUCT))  // 凭证部分的长度
#define CONTROLLEN (RIGHTSLEN + CREDSLEN)               // 控制缓冲区的总长度

// cmptr是一个指向cmsghdr结构体的指针,用于动态分配控制缓冲区
static struct cmsghdr *cmptr = NULL; /* malloc'ed first time */

/*
 * Pass a file descriptor to another process.
 * If fd<0, then -fd is sent back instead as the error status.
 */

/*
 * 将文件描述符传递给另一个进程.
 * 如果fd<0,则返回-fd作为错误状态.
 */

int send_fd(int fd, int fd_to_send)
{
    // 定义CREDSTRUCT结构体指针,用于存储凭证结构体信息
    struct CREDSTRUCT *credp;

    // 定义cmsghdr结构体指针,用于存储消息头信息
    struct cmsghdr *cmp;

    // 定义iovec结构体数组,用于存储消息的I/O向量信息,这里只有一个元素
    struct iovec iov[1];

    // 定义msghdr结构体,用于存储完整的消息信息
    struct msghdr msg;

    // 定义一个2字节的缓冲区buf,用于send_fd/recv_ufd协议
    char buf[2];

    // 初始化iovec结构体数组的第一个元素
    // iov[0].iov_base 指向缓冲区buf的起始地址
    // iov[0].iov_len  设置为2,表示缓冲区buf中要发送的数据长度为2字节
    iov[0].iov_base = buf;
    iov[0].iov_len  = 2;

    // 将iovec结构体数组的指针赋值给msg.msg_iov
    // 这样msg.msg_iov就指向了iov数组,用于后续发送或接收数据
    msg.msg_iov = iov;

    // 设置msg.msg_iovlen为1,表示msg.msg_iov指向的iovec数组中有1个元素
    msg.msg_iovlen = 1;

    // msg.msg_name设置为NULL,表示消息没有特定的地址
    msg.msg_name = NULL;

    // msg.msg_namelen设置为0,因为msg.msg_name是NULL
    msg.msg_namelen = 0;

    // msg.msg_flags设置为0,表示没有特殊的消息标志
    msg.msg_flags = 0;

    // 如果fd_to_send小于0,设置消息控制信息为空,并将buf[1]设置为-fd_to_send作为错误状态
    if (fd_to_send < 0)
    {
        msg.msg_control    = NULL;         // 消息控制信息指针设置为空
        msg.msg_controllen = 0;            // 消息控制长度设置为0
        buf[1]             = -fd_to_send;  // 设置buf[1]为-fd_to_send,非零状态表示错误
        if (buf[1] == 0)                   // 如果buf[1]为0,设置为1,防止-256等数值破坏协议
            buf[1] = 1;
    }
    else
    {
        // 如果cmptr为空,则分配内存
        if (cmptr == NULL && (cmptr = malloc(CONTROLLEN)) == NULL)
            return (-1);  // 内存分配失败,返回-1

        msg.msg_control        = cmptr;       // 设置消息控制信息指针
        msg.msg_controllen     = CONTROLLEN;  // 设置消息控制长度
        cmp                    = cmptr;       // 初始化cmp指针
        cmp->cmsg_level        = SOL_SOCKET;  // 设置控制消息级别为SOL_SOCKET
        cmp->cmsg_type         = SCM_RIGHTS;  // 设置控制消息类型为SCM_RIGHTS
        cmp->cmsg_len          = RIGHTSLEN;   // 设置控制消息长度
        *(int *)CMSG_DATA(cmp) = fd_to_send;  // 设置要传递的文件描述符

        cmp             = CMSG_NXTHDR(&msg, cmp);               // 移动cmp指针到下一个控制消息头
        cmp->cmsg_level = SOL_SOCKET;                           // 设置控制消息级别为SOL_SOCKET
        cmp->cmsg_type  = SCM_CREDTYPE;                         // 设置控制消息类型为SCM_CREDTYPE
        cmp->cmsg_len   = CREDSLEN;                             // 设置控制消息长度
        credp           = (struct CREDSTRUCT *)CMSG_DATA(cmp);  // 获取凭证数据指针

#if defined(SCM_CREDENTIALS)
        credp->uid = geteuid();  // 获取有效用户ID
        credp->gid = getegid();  // 获取有效组ID
        credp->pid = getpid();   // 获取进程ID
#endif
        buf[1] = 0;  // 设置buf[1]为0,表示状态OK
    }

    // 初始化缓冲区的第一个字节为0,作为recv_ufd()函数的空字节标志
    buf[0] = 0;

    // 发送消息,如果发送的消息数量不等于2,则返回-1表示失败
    // sendmsg函数用于通过socket发送消息,msg是要发送的消息结构体,0表示没有特定的标志
    if (sendmsg(fd, &msg, 0) != 2)
        return (-1);

    // 消息发送成功,返回0表示成功
    return (0);
}
