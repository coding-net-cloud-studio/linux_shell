#include "apue.h"        // 引入APUE库头文件
#include <netdb.h>       // 引入网络服务数据库头文件,用于解析主机名和服务名
#include <errno.h>       // 引入错误号头文件,用于获取系统调用的错误代码
#include <sys/socket.h>  // 引入套接字编程所需的头文件

#define BUFLEN  128  // 定义缓冲区长度为128字节
#define TIMEOUT 20   // 定义超时时间为20秒

// sigalrm 函数是一个信号处理函数,用于处理定时器信号 SIGALRM.
// 参数 signo 是接收到的信号编号.
// 注意:当前函数体为空,可能需要根据实际需求添加相应的处理逻辑.
void sigalrm(int signo)
{
    // TODO: 添加信号处理逻辑
}

void print_uptime(int sockfd, struct addrinfo *aip)
{
    int  n;            // 定义整型变量n,用于存储recvfrom函数的返回值
    char buf[BUFLEN];  // 定义字符数组buf,用于存储接收到的数据

    buf[0] = 0;  // 初始化buf数组的第一个元素为0
    // 发送一个长度为1的buf数组到指定的地址和端口,如果发送失败则调用err_sys函数报告错误
    if (sendto(sockfd, buf, 1, 0, aip->ai_addr, aip->ai_addrlen) < 0)
        err_sys("sendto error");
    alarm(TIMEOUT);  // 设置一个定时器,超时时间为TIMEOUT
    // 接收来自任意地址的数据,最多接收BUFLEN个字节,如果接收失败则根据errno的值进行处理
    if ((n = recvfrom(sockfd, buf, BUFLEN, 0, NULL, NULL)) < 0)
    {
        if (errno != EINTR)     // 如果错误不是由于信号中断引起的
            alarm(0);           // 取消定时器
        err_sys("recv error");  // 报告接收错误
    }
    alarm(0);                      // 取消定时器
    write(STDOUT_FILENO, buf, n);  // 将接收到的数据写入标准输出
}

// main函数是程序的入口点,它处理命令行参数,设置信号处理,并尝试解析主机名以获取地址信息.
int main(int argc, char *argv[])
{
    // 定义用于存储地址信息的链表指针和当前处理的地址信息结构体指针
    struct addrinfo *ailist, *aip;
    // 定义用于getaddrinfo函数的提示结构体
    struct addrinfo hint;
    // 定义套接字文件描述符和错误码
    int sockfd, err;
    // 定义信号处理结构体
    struct sigaction sa;

    // 检查命令行参数数量,如果不是2个(程序名和主机名),则打印使用方法并退出
    if (argc != 2)
        err_quit("usage: ruptime hostname");

    // 设置SIGALRM信号的处理函数为sigalrm
    sa.sa_handler = sigalrm;
    sa.sa_flags   = 0;
    sigemptyset(&sa.sa_mask);
    // 安装信号处理函数,如果失败则打印错误信息并退出
    if (sigaction(SIGALRM, &sa, NULL) < 0)
        err_sys("sigaction error");

    // 初始化hint结构体,设置套接字类型为UDP
    // 初始化地址信息结构体
    memset(&hint, 0, sizeof(hint));  // 清零hint结构体,确保所有成员都是未定义的

    // 设置地址信息结构体的成员
    hint.ai_socktype  = SOCK_DGRAM;  // 设置socket类型为UDP
    hint.ai_canonname = NULL;        // 规范主机名指针,这里不需要所以设置为NULL
    hint.ai_addr      = NULL;        // 地址信息指针,这里不需要所以设置为NULL
    hint.ai_next      = NULL;        // 指向下一个地址信息结构体的指针,这里不需要所以设置为NULL

    // 使用getaddrinfo函数解析主机名,如果失败则打印错误信息并退出
    // 如果从命令行参数获取地址信息失败,则调用err_quit函数输出错误信息并退出程序
    // getaddrinfo函数尝试将主机名和服务名映射到一个地址,通过hint参数提供期望的地址族等信息
    // &ailist是一个指向addrinfo结构体链表的指针,用于存储解析结果
    // err_quit函数用于打印错误信息并退出程序
    // gai_strerror函数将getaddrinfo返回的错误码转换为可读的字符串
    if ((err = getaddrinfo(argv[1], "ruptime", &hint, &ailist)) != 0)
        err_quit("getaddrinfo error: %s", gai_strerror(err));

    // 遍历地址信息链表
    for (aip = ailist; aip != NULL; aip = aip->ai_next)
    {
        // 尝试创建套接字,如果失败则记录错误码
        if ((sockfd = socket(aip->ai_family, SOCK_DGRAM, 0)) < 0)
        {
            err = errno;
        }
        else
        {
            // 如果套接字创建成功,则调用print_uptime函数并退出程序
            print_uptime(sockfd, aip);
            exit(0);
        }
    }

    // 如果遍历完地址信息链表都没有成功创建套接字,则打印错误信息并退出
    fprintf(stderr, "can't contact %s: %s\n", argv[1], strerror(err));
    exit(1);
}
