

// 包含APUE库的头文件,该库提供了一套用于UNIX和类UNIX系统的应用程序编程接口.
#include "apue.h"

// 包含socket结构体和相关函数声明的头文件.
#include <sys/socket.h> /* struct msghdr */

// 包含UNIX域套接字相关结构和函数声明的头文件.
#include <sys/un.h>

#if defined(SCM_CREDS)                /* BSD接口 */
#define CREDSTRUCT   cmsgcred         // 定义BSD系统下凭证结构体为cmsgcred
#define CR_UID       cmcred_uid       // 定义BSD系统下用户ID字段为cmcred_uid
#define SCM_CREDTYPE SCM_CREDS        // 定义BSD系统下凭证类型为SCM_CREDS
#elif defined(SCM_CREDENTIALS)        /* Linux接口 */
#define CREDSTRUCT   ucred            // 定义Linux系统下凭证结构体为ucred
#define CR_UID       uid              // 定义Linux系统下用户ID字段为uid
#define CREDOPT      SO_PASSCRED      // 定义Linux系统下传递凭证的选项为SO_PASSCRED
#define SCM_CREDTYPE SCM_CREDENTIALS  // 定义Linux系统下凭证类型为SCM_CREDENTIALS
#else
#error passing credentials is unsupported! // 如果既不是BSD也不是Linux系统,则报错,表示不支持传递凭证
#endif

/* 发送/接收一个文件描述符的控制缓冲区大小 */
#define RIGHTSLEN  CMSG_LEN(sizeof(int))                // 定义控制缓冲区大小,用于存放文件描述符
#define CREDSLEN   CMSG_LEN(sizeof(struct CREDSTRUCT))  // 定义控制缓冲区大小,用于存放凭证信息
#define CONTROLLEN (RIGHTSLEN + CREDSLEN)               // 定义总的控制缓冲区大小,包括文件描述符和凭证信息

static struct cmsghdr *cmptr = NULL; /* 第一次使用时malloc分配 */

/*
 * Receive a file descriptor from a server process.  Also, any data
 * received is passed to (*userfunc)(STDERR_FILENO, buf, nbytes).
 * We have a 2-byte protocol for receiving the fd from send_fd().
 */

/*
 * 从服务器进程接收文件描述符.同时,接收到的任何数据都会传递给 (*userfunc)(STDERR_FILENO, buf, nbytes).
 * 我们有一个2字节的协议用于从send_fd()接收fd.
 *
 * @param fd 接收文件描述符的文件描述符
 * @param uidptr 指向用户ID的指针,用于存储发送方的用户ID
 * @param userfunc 用户定义的函数指针,用于处理接收到的数据
 * @return 成功时返回接收到的文件描述符,失败时返回-1
 */
int recv_ufd(int fd, uid_t *uidptr, ssize_t (*userfunc)(int, const void *, size_t))

{
    // 定义cmsghdr结构体指针cmp,用于存储消息控制信息
    struct cmsghdr *cmp;

    // 定义CREDSTRUCT结构体指针credp,用于存储凭证信息
    struct CREDSTRUCT *credp;

    // 定义字符指针ptr,用于指向数据缓冲区
    char *ptr;

    // 定义字符数组buf,用于存储接收到的数据,大小为MAXLINE
    char buf[MAXLINE];

    // 定义iovec结构体数组iov,用于存储消息的一部分数据及其长度
    struct iovec iov[1];

    // 定义msghdr结构体msg,用于存储消息的所有信息
    struct msghdr msg;

    // 定义整型变量nr,用于存储接收到的字节数
    int nr;

    // 定义整型变量newfd,初始化为-1,用于存储新接收的文件描述符
    int newfd = -1;

    // 定义整型变量status,初始化为-1,用于存储接收状态
    int status = -1;

#if defined(CREDOPT)
    // 定义一个整型变量on并初始化为1,用于设置套接字选项
    const int on = 1;

    // 使用setsockopt函数设置套接字选项,如果失败则返回错误信息并返回-1
    // fd: 套接字描述符
    // SOL_SOCKET: 指定套接字层
    // CREDOPT: 自定义的套接字选项
    // &on: 指向要设置的选项值的指针
    // sizeof(int): 选项值的字节大小
    if (setsockopt(fd, SOL_SOCKET, CREDOPT, &on, sizeof(int)) < 0)
    {
        err_ret("setsockopt error");  // 输出错误信息
        return (-1);                  // 返回错误代码
    }
#endif

    // 以下代码段是一个循环,用于从套接字fd接收消息,并根据消息的状态处理文件描述符或错误信息.
    for (;;)
    {
        // 初始化iovec结构体,用于存储要接收数据的缓冲区信息
        iov[0].iov_base = buf;          // 缓冲区基地址
        iov[0].iov_len  = sizeof(buf);  // 缓冲区大小
        msg.msg_iov     = iov;          // 指向iovec结构体的指针
        msg.msg_iovlen  = 1;            // iovec结构体的数量

        // 初始化msg_name和msg_namelen,这里不需要使用地址信息,所以设置为NULL和0
        msg.msg_name    = NULL;
        msg.msg_namelen = 0;

        // 动态分配内存给cmptr,用于存储控制信息,如果分配失败返回-1
        if (cmptr == NULL && (cmptr = malloc(CONTROLLEN)) == NULL)
            return (-1);

        // 设置msg_control和msg_controllen,用于传递控制信息
        msg.msg_control    = cmptr;       // 控制信息的指针
        msg.msg_controllen = CONTROLLEN;  // 控制信息的大小

        // recvmsg函数尝试从套接字fd接收一个消息,并存储在msg结构体中.
        // 如果接收到的字节数小于0,表示发生了错误,调用err_ret函数输出错误信息并返回-1.
        if ((nr = recvmsg(fd, &msg, 0)) < 0)
        {
            err_ret("recvmsg error");
            return (-1);
        }
        // 如果接收到的字节数为0,表示连接被服务器关闭,调用err_ret函数输出错误信息并返回-1.
        // 如果接收到的字节数为0,表示服务器关闭了连接
        else if (nr == 0)
        {
            err_ret("连接被服务器关闭");  // 输出错误信息
            return (-1);                  // 返回错误代码
        }

        /*
         * See if this is the final data with null & status.  Null
         * is next to last byte of buffer; status byte is last byte.
         * Zero status means there is a file descriptor to receive.
         */
        // 代码段用于解析接收到的消息缓冲区buf,并根据消息的状态来处理文件描述符或错误信息.
        // 如果消息状态为0,则期望接收到一个文件描述符;如果状态不为0,则将-status的值赋给newfd.

        for (ptr = buf; ptr < &buf[nr];)  // 遍历消息缓冲区
        {
            if (*ptr++ == 0)  // 检查当前字节是否为0
            {
                // 检查指针ptr是否指向buf数组的最后一个元素,如果不是,则表示消息格式错误
                if (ptr != &buf[nr - 1])
                    err_dump("消息格式错误");

                // 获取ptr指向的字节的值,并与0xFF进行与操作,以防止符号扩展
                // 这里假设status是一个字节大小的无符号整数
                status = *ptr & 0xFF;

                if (status == 0)  // 状态为0,检查控制长度
                {
                    if (msg.msg_controllen != CONTROLLEN)  // 如果控制长度不等于预定义的CONTROLLEN常量
                        err_dump("status = 0 but no fd");  // 打印错误信息

                    /* 处理控制数据 */
                    for (cmp = CMSG_FIRSTHDR(&msg); cmp != NULL; cmp = CMSG_NXTHDR(&msg, cmp))  // 遍历消息中的控制头
                    {
                        if (cmp->cmsg_level != SOL_SOCKET)  // 如果控制头的级别不是套接字级别,则跳过
                            continue;

                        switch (cmp->cmsg_type)  // 根据控制头的类型进行处理
                        {
                        case SCM_RIGHTS:                     // 如果是SCM_RIGHTS类型,表示传递了文件描述符
                            newfd = *(int *)CMSG_DATA(cmp);  // 获取文件描述符的值
                            break;
                        case SCM_CREDTYPE:                                  // 如果是SCM_CREDTYPE类型,表示传递了凭证信息
                            credp   = (struct CREDSTRUCT *)CMSG_DATA(cmp);  // 获取凭证信息的指针
                            *uidptr = credp->CR_UID;                        // 将用户ID赋值给uidptr指向的变量
                        }
                    }
                }
                else  // 如果status不为0
                {
                    newfd = -status;  // 将-status的值赋给newfd
                }

                nr -= 2;  // nr减去2
            }
        }

        // 如果接收到的字节数大于0,并且用户定义的函数处理这些字节后返回的字节数不等于接收到的字节数,则返回-1.
        // 这意味着在处理数据时可能发生了错误.
        if (nr > 0 && (*userfunc)(STDERR_FILENO, buf, nr) != nr)
            return (-1);

        // 如果状态值大于或等于0,表示最终数据已经到达.
        // 在这种情况下,返回新的文件描述符(newfd),或者如果状态值为负,则返回-status.
        if (status >= 0)
            return (newfd);  // 描述符,或者-status
    }
}
