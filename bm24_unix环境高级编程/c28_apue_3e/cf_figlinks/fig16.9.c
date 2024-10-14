#include "apue.h"        // 引入apue库,提供系统编程接口
#if defined(SOLARIS)     // 如果定义了SOLARIS宏
#include <netinet/in.h>  // 包含网络通信的in.h头文件,用于定义Internet协议
#endif
#include <netdb.h>       // 包含网络数据库操作的头文件,如主机名解析
#include <arpa/inet.h>   // 包含IP地址转换函数的头文件,如inet_addr, inet_ntoa
#if defined(BSD)         // 如果定义了BSD宏
#include <sys/socket.h>  // 包含套接字接口的头文件,用于网络通信
#include <netinet/in.h>  // 再次包含in.h头文件,确保BSD系统下的兼容性
#endif

// 函数功能:打印地址信息家族类型
// 参数:aip - 指向addrinfo结构体的指针,包含地址信息
void print_family(struct addrinfo *aip)
{
    // 打印提示信息
    printf(" family ");
    // 根据地址信息家族类型进行判断并打印相应的字符串
    switch (aip->ai_family)
    {
    case AF_INET:
        printf("inet");  // IPv4地址
        break;
    case AF_INET6:
        printf("inet6");  // IPv6地址
        break;
    case AF_UNIX:
        printf("unix");  // Unix域套接字
        break;
    case AF_UNSPEC:
        printf("unspecified");  // 未指定地址族
        break;
    default:
        printf("unknown");  // 未知地址族
    }
}

// 函数功能:打印地址信息的类型
// 参数:aip - 指向addrinfo结构体的指针
void print_type(struct addrinfo *aip)
{
    // 打印类型标识
    printf(" type ");
    // 根据ai_socktype的值打印具体的套接字类型
    switch (aip->ai_socktype)
    {
    case SOCK_STREAM:
        printf("stream");  // 流式套接字
        break;
    case SOCK_DGRAM:
        printf("datagram");  // 数据报套接字
        break;
    case SOCK_SEQPACKET:
        printf("seqpacket");  // 序列包套接字
        break;
    case SOCK_RAW:
        printf("raw");  // 原始套接字
        break;
    default:
        printf("unknown (%d)", aip->ai_socktype);  // 未知类型
    }
}

// 函数功能:打印地址信息的协议类型
// 参数:aip - 指向addrinfo结构体的指针
void print_protocol(struct addrinfo *aip)
{
    // 打印协议标识
    printf(" protocol ");
    // 根据ai_protocol字段的值打印具体的协议类型
    switch (aip->ai_protocol)
    {
    case 0:
        // 默认协议
        printf("default");
        break;
    case IPPROTO_TCP:
        // TCP协议
        printf("TCP");
        break;
    case IPPROTO_UDP:
        // UDP协议
        printf("UDP");
        break;
    case IPPROTO_RAW:
        // 原始协议
        printf("raw");
        break;
    default:
        // 未知协议
        printf("unknown (%d)", aip->ai_protocol);
    }
}

/**
 * 打印地址信息标志
 * @param aip 指向addrinfo结构体的指针,包含有关网络地址的信息
 */
void print_flags(struct addrinfo *aip)
{
    // 打印标志位提示
    printf("flags");
    // 如果标志位为0,则打印0
    if (aip->ai_flags == 0)
    {
        printf(" 0");
    }
    else
    {
        // 如果设置了AI_PASSIVE标志,则打印passive
        if (aip->ai_flags & AI_PASSIVE)
            printf(" passive");
        // 如果设置了AI_CANONNAME标志,则打印canon
        if (aip->ai_flags & AI_CANONNAME)
            printf(" canon");
        // 如果设置了AI_NUMERICHOST标志,则打印numhost
        if (aip->ai_flags & AI_NUMERICHOST)
            printf(" numhost");
        // 如果设置了AI_NUMERICSERV标志,则打印numserv
        if (aip->ai_flags & AI_NUMERICSERV)
            printf(" numserv");
        // 如果设置了AI_V4MAPPED标志,则打印v4mapped
        if (aip->ai_flags & AI_V4MAPPED)
            printf(" v4mapped");
        // 如果设置了AI_ALL标志,则打印all
        if (aip->ai_flags & AI_ALL)
            printf(" all");
    }
}

/**
 * 主函数,用于解析命令行参数指定的主机名和服务,并打印出相应的地址信息.
 * @param argc 命令行参数的数量
 * @param argv 命令行参数的数组
 * @return 程序退出状态码
 */
int main(int argc, char *argv[])
{
    // 定义用于存储地址信息的链表指针和当前处理的地址信息结构体指针
    struct addrinfo *ailist, *aip;
    // 定义用于getaddrinfo函数的提示结构体
    struct addrinfo hint;
    // 定义用于存储IPv4地址信息的结构体指针
    struct sockaddr_in *sinp;
    // 定义用于存储IP地址字符串的指针
    const char *addr;
    // 定义错误码变量
    int err;
    // 定义用于存储IP地址字符串的缓冲区
    char abuf[INET_ADDRSTRLEN];

    // 检查命令行参数数量是否正确
    if (argc != 3)
        err_quit("usage: %s nodename service", argv[0]);

    // 初始化提示结构体,设置AI_CANONNAME标志
    hint.ai_flags     = AI_CANONNAME;  // 设置AI_CANONNAME标志,请求返回规范主机名
    hint.ai_family    = 0;             // 协议族,0表示任何协议族
    hint.ai_socktype  = 0;             // 套接字类型,0表示任何类型
    hint.ai_protocol  = 0;             // 协议,0表示任何协议
    hint.ai_addrlen   = 0;             // 地址长度,将在后续分配
    hint.ai_canonname = NULL;          // 规范主机名的指针,将在成功解析后填充
    hint.ai_addr      = NULL;          // 地址结构的指针,将在成功解析后填充
    hint.ai_next      = NULL;          // 指向下一个结构的指针,用于链表

    // 调用getaddrinfo函数解析主机名和服务

    // 检查getaddrinfo函数的返回值,如果不为0,表示发生了错误
    if ((err = getaddrinfo(argv[1], argv[2], &hint, &ailist)) != 0)
        // 调用err_quit函数输出错误信息,并退出程序
        // %s会被gai_strerror(err)的返回值替换,该函数将错误码转换为可读的字符串
        err_quit("getaddrinfo error: %s", gai_strerror(err));

    // 遍历地址信息链表
    for (aip = ailist; aip != NULL; aip = aip->ai_next)
    {
        // 打印地址信息的标志
        print_flags(aip);
        // 打印地址信息的地址族
        print_family(aip);
        // 打印地址信息的套接字类型
        print_type(aip);
        // 打印地址信息的协议
        print_protocol(aip);

        // 打印主机名或"-"
        printf("\n\thost %s", aip->ai_canonname ? aip->ai_canonname : "-");

        // 如果地址族是IPv4
        if (aip->ai_family == AF_INET)
        {
            // 将地址信息结构体指针转换为IPv4地址信息结构体指针
            sinp = (struct sockaddr_in *)aip->ai_addr;
            // 将网络字节序的IP地址转换为字符串形式
            addr = inet_ntop(AF_INET, &sinp->sin_addr, abuf, INET_ADDRSTRLEN);
            // 打印IP地址或"unknown"
            printf(" address %s", addr ? addr : "unknown");
            // 打印端口号
            printf(" port %d", ntohs(sinp->sin_port));
        }

        // 换行
        printf("\n");
    }

    // 正常退出程序
    exit(0);
}
