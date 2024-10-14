#include "opend.h"
#include <fcntl.h>

// handle_request 函数用于处理客户端请求.
// 参数:
//   buf: 包含客户端请求数据的缓冲区.
//   nread: 缓冲区中读取的字节数.
//   clifd: 客户端的文件描述符.
//   uid: 客户端的用户ID.
void handle_request(char *buf, int nread, int clifd, uid_t uid)
{
    // 从文件: request.c

    // 定义一个新的文件描述符
    int newfd;

    // 检查读取的缓冲区是否以空字符结尾
    if (buf[nread - 1] != 0)
    {
        // 如果不是,格式化错误消息并发送错误响应
        // 使用snprintf函数格式化错误消息,将错误信息写入errmsg缓冲区.
        // %d用于打印整数uid,%*.*s用于打印最多nread个字符的字符串buf,且不超出MAXLINE - 1的长度限制.
        // 这个错误消息表示接收到的来自uid的请求没有以空字符结尾.
        snprintf(errmsg, MAXLINE - 1, "request from uid %d not null terminated: %*.*s\n", uid, nread, nread, buf);

        // 调用send_err函数发送错误消息到客户端连接的文件描述符clifd.
        // 第二个参数-1表示发生了错误,errmsg包含了具体的错误信息.
        send_err(clifd, -1, errmsg);

        return;  // 结束函数执行
    }

    // 如果缓冲区以空字符结尾,记录请求信息
    log_msg("request: %s, from uid %d", buf, uid);  // 记录请求内容和用户ID

    /* 解析命令行参数,设置选项 */
    // 检查从buf中解析客户端参数的结果
    // 如果解析参数失败(返回值小于0),则执行以下操作:
    // 1. 通过clifd发送错误码-1和错误信息errmsg
    // 2. 记录错误信息errmsg到日志
    // 3. 返回,不执行后续代码
    if (buf_args(buf, cli_args) < 0)
    {
        // 如果解析参数失败,发送错误信息并记录日志
        send_err(clifd, -1, errmsg);  // 发送错误信息
        log_msg(errmsg);              // 记录日志
        return;                       // 返回,终止函数执行
    }

    /* 尝试打开文件 */
    // 打开指定路径的文件,根据oflag参数指定的模式
    // 如果打开文件失败,构造包含错误信息的字符串
    // 然后通过send_err函数发送错误信息给客户端,并记录日志
    if ((newfd = open(pathname, oflag)) < 0)
    {
        // 如果打开文件失败,构造错误信息并发送错误,然后记录日志
        snprintf(errmsg, MAXLINE - 1, "can't open %s: %s\n", pathname, strerror(errno));  // 构造错误信息
        send_err(clifd, -1, errmsg);                                                      // 发送错误信息给客户端
        log_msg(errmsg);                                                                  // 记录错误日志
        return;                                                                           // 返回,不继续执行后续代码
    }

    /* 发送文件描述符 */
    if (send_fd(clifd, newfd) < 0)
        // 如果发送文件描述符失败,记录系统日志
        log_sys("send_fd error");
    else
        // 发送成功,记录日志
        log_msg("sent fd %d over fd %d for %s", newfd, clifd, pathname);
    // 关闭已发送的文件描述符,因为我们已经完成了描述符的使用
    close(newfd);
}
