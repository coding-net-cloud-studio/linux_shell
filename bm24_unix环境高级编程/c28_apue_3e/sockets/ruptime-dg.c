#include "apue.h"        // 引入APUE库的头文件
#include <netdb.h>       // 包含网络数据库操作的头文件
#include <errno.h>       // 包含错误码定义的头文件
#include <sys/socket.h>  // 包含套接字操作的头文件

#define BUFLEN  128  // 定义缓冲区长度为128字节
#define TIMEOUT 20   // 定义超时时间为20秒

// 信号处理函数,用于处理定时器超时信号
void sigalrm(int signo)
{
}

// 打印系统运行时间的函数
// sockfd: 套接字描述符
// aip: 地址信息结构体指针
void print_uptime(int sockfd, struct addrinfo *aip)
{
    int  n;            // 接收数据的字节数
    char buf[BUFLEN];  // 缓冲区,用于存储接收到的数据

    buf[0] = 0;  // 初始化缓冲区
    // 发送一个空数据包到指定的地址
    if (sendto(sockfd, buf, 1, 0, aip->ai_addr, aip->ai_addrlen) < 0)
        err_sys("sendto error");  // 发送失败时调用错误处理函数
    alarm(TIMEOUT);               // 设置定时器
    // 接收来自任意地址的数据
    if ((n = recvfrom(sockfd, buf, BUFLEN, 0, NULL, NULL)) < 0)
    {
        if (errno != EINTR)     // 如果错误不是由于信号中断引起的
            alarm(0);           // 取消定时器
        err_sys("recv error");  // 接收失败时调用错误处理函数
    }
    alarm(0);                      // 接收到数据后取消定时器
    write(STDOUT_FILENO, buf, n);  // 将接收到的数据写入标准输出
}
// main函数是程序的入口点,它处理命令行参数,设置信号处理,并尝试解析主机名以获取地址信息.
int main(int argc, char *argv[])
{
    // 定义用于存储地址信息的链表和当前处理的地址信息结构体
    struct addrinfo *ailist, *aip;
    // 定义用于getaddrinfo函数的提示结构体
    struct addrinfo hint;
    // 定义套接字描述符和错误代码
    int sockfd, err;
    // 定义信号处理结构体
    struct sigaction sa;

    // 检查命令行参数的数量,如果不是2个(程序名和主机名),则打印使用方法并退出
    if (argc != 2)
        err_quit("usage: ruptime hostname");

    // 设置SIGALRM信号的处理函数为sigalrm
    sa.sa_handler = sigalrm;
    sa.sa_flags   = 0;
    sigemptyset(&sa.sa_mask);
    // 安装信号处理函数,如果失败则打印错误并退出
    if (sigaction(SIGALRM, &sa, NULL) < 0)
        err_sys("sig_action error");

    // 初始化提示结构体
    // 初始化地址信息结构体hint
    memset(&hint, 0, sizeof(hint));

    // 设置hint结构体中的ai_socktype字段为SOCK_DGRAM,指定套接字类型为UDP
    hint.ai_socktype = SOCK_DGRAM;

    // 设置hint结构体中的ai_canonname字段为NULL,表示不关心规范主机名
    hint.ai_canonname = NULL;

    // 设置hint结构体中的ai_addr字段为NULL,表示地址信息将在后续步骤中设置
    hint.ai_addr = NULL;

    // 设置hint结构体中的ai_next字段为NULL,表示当前结构体是链表的最后一个节点
    hint.ai_next = NULL;

    // 使用getaddrinfo函数解析主机名,如果失败则打印错误并退出
    if ((err = getaddrinfo(argv[1], "ruptime", &hint, &ailist)) != 0)
        err_quit("getaddrinfo error: %s", gai_strerror(err));

    // 遍历地址信息链表,尝试创建套接字并调用print_uptime函数
    // 遍历地址信息列表
    for (aip = ailist; aip != NULL; aip = aip->ai_next)
    {
        // 尝试创建UDP套接字,如果失败则记录错误代码
        if ((sockfd = socket(aip->ai_family, SOCK_DGRAM, 0)) < 0)
        {
            err = errno;  // 记录失败时的错误代码
        }
        else
        {
            // 如果套接字创建成功,则调用print_uptime函数打印系统运行时间,并退出程序
            // TODO: 确保print_uptime函数能够正确处理sockfd和aip参数
            print_uptime(sockfd, aip);
            exit(0);  // 成功执行后退出程序
        }
    }

    // 如果遍历完所有地址信息都无法创建套接字,则打印错误信息并退出程序
    fprintf(stderr, "can't contact %s: %s\n", argv[1], strerror(err));
    exit(1);
}
