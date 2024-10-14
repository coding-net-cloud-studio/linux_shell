#include "opend.h"  // 包含自定义头文件opend.h,可能定义了一些打开文件的函数或宏
#include <fcntl.h>  // 包含标准库头文件fcntl.h,提供了文件控制操作的功能

// handle_request 函数处理从文件描述符 fd 读取的请求数据.
// 参数:
//   buf: 包含读取数据的缓冲区.
//   nread: 缓冲区中读取的字节数.
//   fd: 文件描述符,表示数据来源.
void handle_request(char *buf, int nread, int fd)
{
    // 定义一个新的文件描述符
    int newfd;

    // 检查请求缓冲区是否以空字符结尾,如果不是,则发送错误消息并返回
    // 检查从请求中读取的数据是否以空字符结尾
    if (buf[nread - 1] != 0)
    {
        // 如果不是,格式化错误消息,指示请求未以空字符结尾
        // %*.*s 是一个格式说明符,用于打印最多 nread 个字符的字符串,每个字符都使用 * 作为宽度
        snprintf(errmsg, MAXLINE - 1, "request not null terminated: %*.*s\n", nread, nread, buf);

        // 发送错误消息并返回
        send_err(fd, -1, errmsg);
        return;
    }

    // 解析请求参数并设置选项,如果解析失败,则发送错误消息并返回
    // request.c

    // 检查从buf中解析命令行参数并设置选项的结果
    // 如果buf_args函数返回值小于0,表示解析参数失败
    if (buf_args(buf, cli_args) < 0)
    {
        // 解析参数失败,发送错误信息
        // send_err函数用于向文件描述符fd发送错误码和错误信息
        send_err(fd, -1, errmsg);
        // 结束当前函数执行
        return;
    }

    // 尝试打开文件,如果失败,则发送错误消息并返回
    // 打开文件,如果失败则发送错误信息并返回
    if ((newfd = open(pathname, oflag)) < 0)  // 尝试使用指定的标志打开文件
    {
        snprintf(errmsg, MAXLINE - 1, "can't open %s: %s\n", pathname, strerror(errno));  // 格式化错误信息
        send_err(fd, -1, errmsg);                                                         // 发送错误信息到指定的文件描述符
        return;                                                                           // 返回,不继续执行后续代码
    }

    // 发送文件描述符,如果发送失败,则记录系统错误
    if (send_fd(fd, newfd) < 0) /* send the descriptor */
        err_sys("send_fd error");
    // 关闭已发送的文件描述符
    close(newfd); /* we're done with descriptor */
}
