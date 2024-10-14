#include "apue.h"        // 引入APUE库,提供系统编程接口
#include <sys/socket.h>  // 引入socket库,提供网络通信功能

// 定义CREDSTRUCT和SCM_CREDTYPE,根据不同的操作系统接口
#if defined(SCM_CREDS)                /* BSD接口 */
#define CREDSTRUCT   cmsgcred         // BSD系统使用cmsgcred结构体
#define SCM_CREDTYPE SCM_CREDS        // BSD系统使用SCM_CREDS类型
#elif defined(SCM_CREDENTIALS)        /* Linux接口 */
#define CREDSTRUCT   ucred            // Linux系统使用ucred结构体
#define SCM_CREDTYPE SCM_CREDENTIALS  // Linux系统使用SCM_CREDENTIALS类型
#else
#error passing credentials is unsupported!  // 如果不支持传递凭证,则报错
#endif

// 定义控制缓冲区的大小,用于发送/接收一个文件描述符
#define RIGHTSLEN  CMSG_LEN(sizeof(int))                // 定义RIGHTSLEN为包含int大小的控制消息长度
#define CREDSLEN   CMSG_LEN(sizeof(struct CREDSTRUCT))  // 定义CREDSLEN为包含CREDSTRUCT结构体大小的控制消息长度
#define CONTROLLEN (RIGHTSLEN + CREDSLEN)               // 定义CONTROLLEN为RIGHTSLEN和CREDSLEN之和,即总的控制消息长度

static struct cmsghdr *cmptr = NULL;  // 静态变量cmptr,首次使用时malloc分配内存

/*
 * Pass a file descriptor to another process.
 * If fd<0, then -fd is sent back instead as the error status.
 */
int send_fd(int fd, int fd_to_send)
{
    // 定义CREDSTRUCT结构体指针,用于存储凭证信息
    struct CREDSTRUCT *credp;

    // 定义cmsghdr结构体指针,用于存储消息控制头信息
    struct cmsghdr *cmp;

    // 定义iovec结构体数组,用于存储消息的一部分
    struct iovec iov[1];

    // 定义msghdr结构体,用于存储消息头信息
    struct msghdr msg;

    // 定义字符数组buf,用于存储send_fd/recv_ufd协议的2字节协议
    char buf[2];

    // 初始化iovec结构体的第一个元素,设置其基地址为buf,长度为2字节
    iov[0].iov_base = buf;
    iov[0].iov_len  = 2;

    // 设置msg的消息向量指针指向iov数组,消息向量的数量为1
    msg.msg_iov    = iov;
    msg.msg_iovlen = 1;

    // 设置msg的消息名称指针为NULL,表示不需要特定的地址
    msg.msg_name    = NULL;
    msg.msg_namelen = 0;

    // 设置msg的消息标志位为0,表示没有特殊标志
    msg.msg_flags = 0;

    // 如果fd_to_send小于0,表示发送文件描述符时出错
    if (fd_to_send < 0)
    {
        // 设置消息控制信息为空
        msg.msg_control = NULL;
        // 消息控制长度为0
        msg.msg_controllen = 0;
        // 将错误状态码存入buf的第二个字节
        buf[1] = -fd_to_send;
        // 如果buf[1]为0,表示错误状态码为-256等,需要修正协议
        if (buf[1] == 0)
            buf[1] = 1;
    }
    else
    {
        // 如果cmptr为空,则分配内存
        if (cmptr == NULL && (cmptr = malloc(CONTROLLEN)) == NULL)
            return (-1);  // 内存分配失败,返回-1
        // 设置消息控制信息和长度
        msg.msg_control    = cmptr;
        msg.msg_controllen = CONTROLLEN;
        cmp                = cmptr;
        // 设置控制消息级别为SOL_SOCKET
        cmp->cmsg_level = SOL_SOCKET;
        // 设置控制消息类型为SCM_RIGHTS,用于传递文件描述符
        cmp->cmsg_type = SCM_RIGHTS;
        // 设置控制消息长度
        cmp->cmsg_len = RIGHTSLEN;
        // 将要传递的文件描述符存入控制消息数据区
        *(int *)CMSG_DATA(cmp) = fd_to_send;
        // 移动到下一个控制消息头
        cmp = CMSG_NXTHDR(&msg, cmp);
        // 设置控制消息级别为SOL_SOCKET
        cmp->cmsg_level = SOL_SOCKET;
        // 设置控制消息类型为SCM_CREDTYPE,用于传递凭证信息
        cmp->cmsg_type = SCM_CREDTYPE;
        // 设置控制消息长度
        cmp->cmsg_len = CREDSLEN;
        credp         = (struct CREDSTRUCT *)CMSG_DATA(cmp);
        // 如果定义了SCM_CREDENTIALS,设置凭证信息
#if defined(SCM_CREDENTIALS)
        credp->uid = geteuid();  // 用户ID
        credp->gid = getegid();  // 组ID
        credp->pid = getpid();   // 进程ID
#endif
        // 设置buf的第二个字节为0,表示操作成功
        buf[1] = 0;
    }

    // sendfd2.c

    // 设置buf的第一个字节为0,作为recv_ufd()函数的空字节标志
    buf[0] = 0;

    // 使用sendmsg函数发送消息,如果发送的消息数量不等于2,则返回-1
    // sendmsg函数用于发送一个消息,包括一个或多个数据缓冲区
    // 参数fd是文件描述符,&msg指向要发送的消息结构体,0表示使用默认标志
    if (sendmsg(fd, &msg, 0) != 2)
        return (-1);

    // 如果消息发送成功,返回0
    return (0);
}
