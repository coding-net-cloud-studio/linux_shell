#include "opend.h"  // 包含自定义的头文件,可能包含函数声明或宏定义
#include <fcntl.h>  // 包含标准库头文件,提供文件控制功能,如open, close等

/**
 * @brief 处理客户端的请求.
 *
 * 这个函数解析客户端发送的请求,打开指定的文件,并将文件描述符发送回客户端.
 *
 * @param buf 指向包含客户端请求的缓冲区的指针.
 * @param nread 从客户端读取的字节数.
 * @param clifd 客户端的文件描述符.
 * @param uid 客户端的用户 ID.
 * @return 不返回任何值.
 */
void handle_request(char *buf, int nread, int clifd, uid_t uid)
{
    int newfd;

    // 检查请求是否以空字符结尾
    if (buf[nread - 1] != 0)
    {
        // 格式化错误消息,指出请求未以空字符结尾
        snprintf(errmsg, MAXLINE - 1, "request from uid %d not null terminated: %*.*s\n", uid, nread, nread, buf);
        // 将错误消息发送给客户端
        send_err(clifd, -1, errmsg);
        return;
    }
    // 记录请求日志
    log_msg("request: %s, from uid %d", buf, uid);

    // 解析请求参数,设置选项
    if (buf_args(buf, cli_args) < 0)
    {
        // 将错误消息发送给客户端
        send_err(clifd, -1, errmsg);
        // 记录错误日志
        log_msg(errmsg);
        return;
    }

    // 尝试打开请求的文件
    if ((newfd = open(pathname, oflag)) < 0)
    {
        // 格式化错误消息,指出无法打开文件
        snprintf(errmsg, MAXLINE - 1, "can't open %s: %s\n", pathname, strerror(errno));
        // 将错误消息发送给客户端
        send_err(clifd, -1, errmsg);
        // 记录错误日志
        log_msg(errmsg);
        return;
    }

    // 将打开的文件描述符发送给客户端
    if (send_fd(clifd, newfd) < 0)
        // 记录发送文件描述符错误
        log_sys("send_fd error");
    // 记录发送文件描述符成功的日志
    log_msg("sent fd %d over fd %d for %s", newfd, clifd, pathname);
    // 关闭文件描述符,因为我们已经完成了对它的操作
    close(newfd);
}
