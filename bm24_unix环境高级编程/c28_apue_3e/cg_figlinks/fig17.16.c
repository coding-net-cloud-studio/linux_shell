#include "apue.h"                            // 引入apue库的头文件,可能包含一些常用的系统编程接口和宏定义
#include <sys/socket.h> /* struct msghdr */  // 引入socket库的头文件,包含了用于网络通信的结构体和方法,例如struct msghdr用于消息传递
#include <sys/un.h>                          // 引入Unix域套接字相关的头文件,用于本地进程间通信

// 定义了用于BSD和Linux接口的凭证结构体和相关宏
#if defined(SCM_CREDS)                /* BSD接口 */
#define CREDSTRUCT   cmsgcred         // 定义凭证结构体为cmsgcred
#define CR_UID       cmcred_uid       // 定义用户ID字段为cmcred_uid
#define SCM_CREDTYPE SCM_CREDS        // 定义凭证类型为SCM_CREDS
#elif defined(SCM_CREDENTIALS)        /* Linux接口 */
#define CREDSTRUCT   ucred            // 定义凭证结构体为ucred
#define CR_UID       uid              // 定义用户ID字段为uid
#define CREDOPT      SO_PASSCRED      // 定义传递凭证的选项为SO_PASSCRED
#define SCM_CREDTYPE SCM_CREDENTIALS  // 定义凭证类型为SCM_CREDENTIALS
#else
#error passing credentials is unsupported! // 如果不支持传递凭证,则报错
#endif

// 定义控制缓冲区的大小,用于发送/接收一个文件描述符
#define RIGHTSLEN  CMSG_LEN(sizeof(int))                // 定义权利长度
#define CREDSLEN   CMSG_LEN(sizeof(struct CREDSTRUCT))  // 定义凭证长度
#define CONTROLLEN (RIGHTSLEN + CREDSLEN)               // 定义控制缓冲区的总长度

static struct cmsghdr *cmptr = NULL;  // 静态指针,首次使用时malloc分配内存

/*
 * Receive a file descriptor from a server process.  Also, any data
 * received is passed to (*userfunc)(STDERR_FILENO, buf, nbytes).
 * We have a 2-byte protocol for receiving the fd from send_fd().
 */

/*
 * 从服务器进程接收文件描述符.同时,任何接收到的数据都将传递给(*userfunc)(STDERR_FILENO, buf, nbytes).
 * 我们有一个2字节的协议来从send_fd()接收文件描述符.
 */

// 函数 recv_ufd
// 功能:从文件描述符 fd 接收数据,并根据用户提供的函数 userfunc 进行处理
// 参数:
// - fd:文件描述符,用于接收数据的通道
// - uidptr:指向用户 ID 的指针,用于存储接收数据的用户 ID
// - userfunc:用户定义的函数指针,用于处理接收到的数据
// 返回值:成功时返回接收到的字节数,失败时返回 -1
int recv_ufd(int fd, uid_t *uidptr, ssize_t (*userfunc)(int, const void *, size_t))
{
    // 定义cmsghdr结构体指针cmp,用于存储消息控制信息
    struct cmsghdr *cmp;

    // 定义CREDSTRUCT结构体指针credp,可能用于存储凭证信息
    struct CREDSTRUCT *credp;

    // 定义字符指针ptr,用于指向数据缓冲区
    char *ptr;

    // 定义字符数组buf,大小为MAXLINE,用于存储接收或发送的数据
    char buf[MAXLINE];

    // 定义iovec结构体数组iov,大小为1,用于存储消息的一部分
    struct iovec iov[1];

    // 定义msghdr结构体msg,用于存储消息的所有信息
    struct msghdr msg;

    // 定义整型变量nr,可能用于存储消息的数量或其他相关状态
    int nr;

    // 定义整型变量newfd,初始化为-1,可能用于存储新打开的文件描述符
    int newfd = -1;

    // 定义整型变量status,初始化为-1,可能用于存储函数执行的状态码
    int status = -1;

#if defined(CREDOPT)   // 如果定义了CREDOPT宏
    const int on = 1;  // 定义一个整型常量on,值为1

    // 设置套接字选项,启用CREDOPT选项
    // 设置套接字选项,启用或禁用特定的套接字选项
    // 参数fd: 套接字描述符
    // 参数level: 选项定义的协议层,SOL_SOCKET表示选项在套接字级别
    // 参数optname: 套接字选项名,CREDOPT用于控制TCP的连接重试行为
    // 参数optval: 指向要设置的选项值的指针
    // 参数optlen: optval指向的值的大小
    // 返回值: 成功返回0,失败返回-1,并设置errno
    if (setsockopt(fd, SOL_SOCKET, CREDOPT, &on, sizeof(int)) < 0)

    {
        err_ret("setsockopt error");  // 如果设置套接字选项失败,输出错误信息
        return (-1);                  // 返回-1表示失败
    }
#endif

    // 无限循环,通常用于程序的主事件循环或等待状态
    for (;;)
    {
        // 初始化iovec结构体,用于存储接收数据的缓冲区信息
        iov[0].iov_base = buf;          // 设置缓冲区起始地址
        iov[0].iov_len  = sizeof(buf);  // 设置缓冲区大小
        msg.msg_iov     = iov;          // 将iovec结构体指针赋值给msg结构体的msg_iov成员
        msg.msg_iovlen  = 1;            // 设置iovec结构体数组的长度

        // 初始化msg_name和msg_namelen,这里msg_name用于存储地址信息,msg_namelen用于存储地址信息的长度
        msg.msg_name    = NULL;
        msg.msg_namelen = 0;

        // 分配内存给cmptr,用于存储控制信息
        if (cmptr == NULL && (cmptr = malloc(CONTROLLEN)) == NULL)
            return (-1);  // 如果内存分配失败,返回-1

        msg.msg_control    = cmptr;       // 将cmptr指针赋值给msg结构体的msg_control成员
        msg.msg_controllen = CONTROLLEN;  // 设置控制信息的长度

        // 使用recvmsg函数接收数据
        if ((nr = recvmsg(fd, &msg, 0)) < 0)  // 如果recvmsg函数返回值小于0,表示接收消息失败
        {
            err_ret("recvmsg error");  // 输出错误信息
            return (-1);               // 返回-1表示失败
        }
        else if (nr == 0)  // 如果recvmsg函数返回值为0,表示连接被服务器关闭
        {
            err_ret("connection closed by server");  // 输出错误信息
            return (-1);                             // 返回-1表示失败
        }

        /*
         * See if this is the final data with null & status.  Null
         * is next to last byte of buffer; status byte is last byte.
         * Zero status means there is a file descriptor to receive.
         */
        // 以下是对选定代码的注释:

        // 遍历缓冲区中的数据
        for (ptr = buf; ptr < &buf[nr];)
        {
            // 检查当前字节是否为0
            if (*ptr++ == 0)
            {
                // 如果不是最后一个字节,报告格式错误
                if (ptr != &buf[nr - 1])
                    err_dump("message format error");

                // 获取状态字节,并防止符号扩展
                status = *ptr & 0xFF;

                // 如果状态为0
                if (status == 0)
                {
                    // 检查控制数据长度是否符合预期
                    if (msg.msg_controllen != CONTROLLEN)
                        err_dump("status = 0 but no fd");

                    // 遍历消息中的所有控制头
                    for (cmp = CMSG_FIRSTHDR(&msg); cmp != NULL; cmp = CMSG_NXTHDR(&msg, cmp))
                    {
                        // 检查控制头的级别是否为套接字级别
                        if (cmp->cmsg_level != SOL_SOCKET)
                            continue;  // 如果不是,跳过当前迭代

                        // 根据控制头的类型进行不同的处理
                        switch (cmp->cmsg_type)
                        {
                        case SCM_RIGHTS:
                            // 如果类型是SCM_RIGHTS,获取文件描述符
                            newfd = *(int *)CMSG_DATA(cmp);
                            break;
                        case SCM_CREDTYPE:
                            // 如果类型是SCM_CREDTYPE,获取凭证信息
                            credp   = (struct CREDSTRUCT *)CMSG_DATA(cmp);
                            *uidptr = credp->CR_UID;  // 将用户ID复制到指针指向的位置
                        }
                    }
                }
                else
                {
                    // 如果状态不为0,将状态转换为负数作为文件描述符
                    newfd = -status;
                }
                // 减少计数器,跳过已处理的数据
                nr -= 2;
            }
        }

        // 检查读取的字节数是否大于0,并且用户定义的函数(*userfunc)对标准错误流(STDERR_FILENO)的处理结果是否与读取的字节数(nr)相等
        // 如果不等,说明处理过程中出现错误,返回-1
        if (nr > 0 && (*userfunc)(STDERR_FILENO, buf, nr) != nr)
            return (-1);

        // 如果状态(status)大于等于0,表示最终数据已经到达
        // 返回新的文件描述符(newfd),或者如果状态为负数则返回-status
        if (status >= 0)
            return (newfd);  // 描述符,或 -status
    }
}
