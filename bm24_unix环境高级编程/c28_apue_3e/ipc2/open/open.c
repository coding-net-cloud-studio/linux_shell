#include "open.h"                        // 引入open.h头文件,可能包含函数声明和定义
#include <sys/uio.h> /* struct iovec */  // 引入sys/uio.h头文件,包含结构体iovec的定义,用于散布/聚集I/O操作

/*
 * 打开文件,通过向连接服务器发送文件名"name"和打开标志"oflag",
 * 然后读回一个文件描述符.
 */
int csopen(char *name, int oflag)
{
    int          len;        // 用于存储发送数据的总长度
    char         buf[12];    // 缓冲区,用于存储将oflag转换为ASCII格式后的字符串
    struct iovec iov[3];     // iovec结构体数组,用于存储要发送的数据块
    static int   csfd = -1;  // 静态变量,存储与连接服务器的文件描述符

    if (csfd < 0)
    { /* 如果还没有连接到连接服务器 */
        if ((csfd = cli_conn(CS_OPEN)) < 0)
        {                               // 尝试建立连接
            err_ret("cli_conn error");  // 连接失败,返回错误信息
            return (-1);                // 返回-1表示失败
        }
    }

    sprintf(buf, " %d", oflag);                                          // 将oflag转换为ASCII字符串格式
    iov[0].iov_base = CL_OPEN " ";                                       // 第一个数据块,包含命令字符串
    iov[0].iov_len  = strlen(CL_OPEN) + 1;                               // 第一个数据块的长度
    iov[1].iov_base = name;                                              // 第二个数据块,包含文件名
    iov[1].iov_len  = strlen(name);                                      // 第二个数据块的长度
    iov[2].iov_base = buf;                                               // 第三个数据块,包含oflag的ASCII字符串
    iov[2].iov_len  = strlen(buf) + 1;                                   // 第三个数据块的长度,包括结尾的空字符
    len             = iov[0].iov_len + iov[1].iov_len + iov[2].iov_len;  // 计算总长度
    if (writev(csfd, &iov[0], 3) != len)
    {                             // 发送数据到连接服务器
        err_ret("writev error");  // 发送失败,返回错误信息
        return (-1);              // 返回-1表示失败
    }

    /* 读回文件描述符;返回的错误由write()处理 */
    return (recv_fd(csfd, write));  // 接收文件描述符并返回
}
