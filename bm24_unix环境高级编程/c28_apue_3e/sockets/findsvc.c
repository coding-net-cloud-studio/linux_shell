#include "apue.h"
#if defined(SOLARIS)
#include <netinet/in.h>  // 包含网络协议套接字地址结构体的定义
#endif
#include <netdb.h>      // 包含主机信息数据库操作函数的定义
#include <arpa/inet.h>  // 包含IP地址转换函数的定义
#if defined(BSD)
#include <sys/socket.h>  // 包含套接字接口函数的定义(BSD系统)
#include <netinet/in.h>  // 再次包含网络协议套接字地址结构体的定义(BSD系统)
#endif

// 注意:代码中存在一个预处理指令 #endif,它似乎没有与之匹配的 #if 或 #ifdef.
// 这可能是一个错误,应该检查并修正以确保预处理指令的正确配对.

// 打印地址信息家族类型
// @param aip 指向addrinfo结构体的指针
void print_family(struct addrinfo *aip)
{
    printf(" family ");  // 打印提示信息
    // 根据地址信息家族类型打印相应的字符串
    switch (aip->ai_family)
    {
    case AF_INET:
        printf("inet");  // IPv4
        break;
    case AF_INET6:
        printf("inet6");  // IPv6
        break;
    case AF_UNIX:
        printf("unix");  // Unix域套接字
        break;
    case AF_UNSPEC:
        printf("unspecified");  // 未指定
        break;
    default:
        printf("unknown");  // 未知类型
    }
}
void print_type(struct addrinfo *aip)
{
    // 打印地址信息类型
    printf(" type ");
    // 根据套接字类型打印相应的描述
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
// 函数功能:打印地址信息中的协议类型
// 参数:aip - 指向addrinfo结构体的指针,包含网络地址信息
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
// findsvc.c

/**
 * 主函数,用于解析主机名和服务名,并打印出相应的地址信息.
 * @param argc 命令行参数的数量
 * @param argv 命令行参数的数组
 * @return 程序退出状态码
 */
int main(int argc, char *argv[])
{
    // 定义地址信息结构体指针和链表头
    struct addrinfo *ailist, *aip;
    // 定义地址信息提示结构体
    struct addrinfo hint;
    // 定义IPv4地址结构体指针
    struct sockaddr_in *sinp;
    // 定义地址字符串指针
    const char *addr;
    // 定义错误码
    int err;
    // 定义用于存储IP地址的缓冲区
    char abuf[INET_ADDRSTRLEN];

    // 检查命令行参数数量是否正确
    if (argc != 3)
        err_quit("usage: %s nodename service", argv[0]);

    // 初始化地址信息提示结构体
    // 初始化addrinfo结构体,用于存储地址信息
    hint.ai_flags     = AI_CANONNAME;  // 设置AI_CANONNAME标志,请求返回规范主机名
    hint.ai_family    = 0;             // 协议族,0表示使用默认协议族
    hint.ai_socktype  = 0;             // 套接字类型,0表示使用默认类型
    hint.ai_protocol  = 0;             // 协议,0表示使用默认协议
    hint.ai_addrlen   = 0;             // 地址长度,初始化为0
    hint.ai_canonname = NULL;          // 规范主机名指针,初始化为NULL
    hint.ai_addr      = NULL;          // 地址信息指针,初始化为NULL
    hint.ai_next      = NULL;          // 指向下一个addrinfo结构的指针,初始化为NULL

    // 获取地址信息
    if ((err = getaddrinfo(argv[1], argv[2], &hint, &ailist)) != 0)
        err_quit("getaddrinfo error: %s", gai_strerror(err));

    // 遍历地址信息链表
    for (aip = ailist; aip != NULL; aip = aip->ai_next)
    {
        // 打印地址信息的标志,家族,类型和协议
        // 打印地址信息标志
        print_flags(aip);  // 输出地址信息标志的详细信息

        // 打印地址族
        print_family(aip);  // 输出地址族的类型

        // 打印套接字类型
        print_type(aip);  // 输出套接字类型的详细信息

        // 打印协议
        print_protocol(aip);  // 输出使用的协议的详细信息

        // 打印主机名
        // 如果ai_canonname不为空,则打印主机名,否则打印"-"
        printf("\n\thost %s", aip->ai_canonname ? aip->ai_canonname : "-");

        // 如果是IPv4地址,则打印地址和端口
        // 检查地址信息协议(ai)的地址族是否为IPv4
        if (aip->ai_family == AF_INET)
        {
            // 将ai_addr转换为IPv4的sockaddr_in结构体指针
            sinp = (struct sockaddr_in *)aip->ai_addr;
            // 将网络字节序的IPv4地址转换为字符串形式
            addr = inet_ntop(AF_INET, &sinp->sin_addr, abuf, INET_ADDRSTRLEN);
            // 打印地址,如果addr不为空则打印addr,否则打印"unknown"
            printf(" address %s", addr ? addr : "unknown");
            // 将网络字节序的端口号转换为主机字节序并打印
            printf(" port %d", ntohs(sinp->sin_port));
        }

        // 换行
        printf("\n");
    }

    // 正常退出程序
    exit(0);
}
