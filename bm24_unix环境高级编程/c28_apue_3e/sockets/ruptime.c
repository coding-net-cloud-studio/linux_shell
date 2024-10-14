#include "apue.h"        // 引入APUE库的头文件
#include <netdb.h>       // 包含网络数据库操作的头文件
#include <errno.h>       // 包含错误码定义的头文件
#include <sys/socket.h>  // 包含套接字操作的头文件

#define BUFLEN 128  // 定义缓冲区长度为128字节

// connect_retry函数尝试连接服务器,最多尝试retry_times次,每次间隔interval秒
extern int connect_retry(int, int, int, const struct sockaddr *, socklen_t);

// print_uptime函数通过sockfd套接字接收服务器的运行时间信息,并将其打印到标准输出
void print_uptime(int sockfd)
{
    int  n;            // n用于存储recv函数的返回值,表示接收到的字节数
    char buf[BUFLEN];  // buf用于存储从服务器接收到的数据

    // 循环接收数据,直到recv函数返回0(连接关闭)或小于0(发生错误)
    while ((n = recv(sockfd, buf, BUFLEN, 0)) > 0)
        write(STDOUT_FILENO, buf, n);  // 将接收到的数据写入标准输出
    if (n < 0)
        err_sys("recv error");  // 如果recv函数返回小于0,打印错误信息并退出程序
}
// ruptime.c

/**
 * 主函数,程序入口点.
 * @param argc 命令行参数的数量
 * @param argv 命令行参数的数组
 * @return 程序退出状态码
 */
int main(int argc, char *argv[])
{
    // 定义地址信息结构体指针和变量
    struct addrinfo *ailist, *aip;
    struct addrinfo  hint;
    // 定义套接字描述符和错误码
    int sockfd, err;

    // 检查命令行参数数量是否正确
    if (argc != 2)
        err_quit("usage: ruptime hostname");

    // 初始化地址信息提示结构体
    memset(&hint, 0, sizeof(hint));
    hint.ai_socktype  = SOCK_STREAM;  // 设置套接字类型为流式套接字
    hint.ai_canonname = NULL;
    hint.ai_addr      = NULL;
    hint.ai_next      = NULL;

    // 获取地址信息
    if ((err = getaddrinfo(argv[1], "ruptime", &hint, &ailist)) != 0)
        err_quit("getaddrinfo error: %s", gai_strerror(err));

    // 遍历地址信息链表,尝试连接
    for (aip = ailist; aip != NULL; aip = aip->ai_next)
    {
        // 尝试连接服务器
        if ((sockfd = connect_retry(aip->ai_family, SOCK_STREAM, 0, aip->ai_addr, aip->ai_addrlen)) < 0)
        {
            // 连接失败,记录错误码
            err = errno;
        }
        else
        {
            // 连接成功,打印服务器运行时间并退出程序
            print_uptime(sockfd);
            exit(0);
        }
    }

    // 所有地址都尝试过后仍然无法连接,退出程序并报告错误
    err_exit(err, "can't connect to %s", argv[1]);
}
