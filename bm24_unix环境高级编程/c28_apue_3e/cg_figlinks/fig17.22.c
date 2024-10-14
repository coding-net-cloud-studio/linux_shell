#include "opend.h"  // 包含自定义的头文件,可能包含函数声明或其他必要的定义
#include <fcntl.h>  // 包含标准库头文件,提供文件控制函数,如open(), close()等

// handle_request 函数处理从文件描述符 fd 读取的请求数据.
// 参数 buf 是一个字符数组,用于存储读取的数据.
// 参数 nread 是实际读取的字节数.
// 参数 fd 是文件描述符.
void handle_request(char *buf, int nread, int fd)
{
    // newfd 是一个整型变量,通常用于表示文件描述符.
    // 在Unix和类Unix系统中,文件描述符是一个非负整数,用于指向内核为每个进程维护的打开文件记录表中的一个条目.
    // 当调用如open,socket等系统调用时,会返回一个新的文件描述符.
    // 这个变量可能会在后续的代码中用于引用打开的文件或网络套接字.
    int newfd;

    // 检查请求是否以空字符结尾
    // 检查从缓冲区读取的数据是否以空字符结尾
    if (buf[nread - 1] != 0)
    {
        // 如果不是,格式化错误消息
        // %*.*s 允许我们打印指定数量的字符
        snprintf(errmsg, MAXLINE - 1, "request not null terminated: %*.*s\n", nread, nread, buf);

        // 发送错误信息到指定的文件描述符
        // send_err函数的第二个参数为-1,通常表示发生了错误
        send_err(fd, -1, errmsg);

        // 返回,不继续执行后续代码
        return;
    }

    // 解析参数并设置选项

    // 检查buf_args函数的结果,如果返回值小于0,表示出现了错误
    if (buf_args(buf, cli_args) < 0)
    {
        // 发送错误信息到文件描述符fd,并附带错误码-1和错误消息errmsg
        send_err(fd, -1, errmsg);  // 发送错误信息
        return;                    // 结束当前函数的执行
    }

    // 打开文件
    // 打开文件,并检查是否成功打开
    if ((newfd = open(pathname, oflag)) < 0)
    {
        // 如果打开文件失败,格式化错误信息
        snprintf(errmsg, MAXLINE - 1, "无法打开 %s: %s\n", pathname, strerror(errno));
        // 发送错误信息到指定的文件描述符
        send_err(fd, -1, errmsg);  // 发送错误信息
        return;                    // 返回,不继续执行后续代码
    }

    // 发送文件描述符
    if (send_fd(fd, newfd) < 0)
        err_sys("send_fd error");  // 发送错误信息
    close(newfd);                  // 关闭文件描述符
}
