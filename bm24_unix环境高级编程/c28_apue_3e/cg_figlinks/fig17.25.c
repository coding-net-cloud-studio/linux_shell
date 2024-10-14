#include "open.h"                        // 包含自定义的头文件open.h,可能包含文件打开相关的函数声明或宏定义
#include <sys/uio.h> /* struct iovec */  // 包含sys/uio.h头文件,该文件定义了用于散布/聚集I/O的结构体iovec

/*
 * Open the file by sending the "name" and "oflag" to the
 * connection server and reading a file descriptor back.
 */
/*
 * 打开文件,通过向连接服务器发送文件名和打开标志,并读取返回的文件描述符.
 */

// csopen函数用于打开一个文件,需要文件名和打开标志
// csopen 函数用于打开一个文件,并返回一个文件描述符.
// 参数:
//   name: 要打开的文件的名称.
//   oflag: 打开文件时的标志,例如只读,写入模式等.
int csopen(char *name, int oflag)
{
    int          len;        // 用于存储数据长度
    char         buf[12];    // 缓冲区,用于存储oflag的ASCII表示
    struct iovec iov[3];     // iovec结构体数组,用于存储要发送的数据块
    static int   csfd = -1;  // 静态变量,存储连接服务器的文件描述符

    // 如果csfd小于0,表示还未连接到连接服务器
    if (csfd < 0)
    {
        // 尝试连接连接服务器
        if ((csfd = cli_conn(CS_OPEN)) < 0)
        {
            err_ret("cli_conn error");  // 连接失败,返回错误信息
            return (-1);                // 返回-1表示失败
        }
    }

    // 将oflag转换为ASCII字符串并存储到buf中
    sprintf(buf, " %d", oflag);

    // 设置iov数组,分别存储命令字符串,文件名和oflag的ASCII表示
    // 初始化iovec数组的第一个元素,用于存储命令字符串"CL_OPEN"和一个空格
    // iov[0].iov_base 指向命令字符串的首地址
    // iov[0].iov_len 是命令字符串的长度加上一个空格的长度和一个null终止符的长度
    iov[0].iov_base = CL_OPEN " ";
    iov[0].iov_len  = strlen(CL_OPEN) + 1;

    // 初始化iovec数组的第二个元素,用于存储文件名
    // iov[1].iov_base 指向文件名的首地址
    // iov[1].iov_len 是文件名的长度
    iov[1].iov_base = name;
    iov[1].iov_len  = strlen(name);

    // 初始化iovec数组的第三个元素,用于存储缓冲区的内容
    // iov[2].iov_base 指向缓冲区的首地址
    // iov[2].iov_len 是缓冲区内容的长度加上一个null终止符的长度
    // 总是发送null终止符是为了确保接收端能够正确解析字符串
    iov[2].iov_base = buf;
    iov[2].iov_len  = strlen(buf) + 1;

    // 计算要发送的总数据长度
    len = iov[0].iov_len + iov[1].iov_len + iov[2].iov_len;

    // 发送数据到连接服务器
    if (writev(csfd, &iov[0], 3) != len)
    {
        err_ret("writev error");  // 发送失败,返回错误信息
        return (-1);              // 返回-1表示失败
    }

    // 读取返回的文件描述符,recv_fd函数处理返回的错误
    return (recv_fd(csfd, write));
}
