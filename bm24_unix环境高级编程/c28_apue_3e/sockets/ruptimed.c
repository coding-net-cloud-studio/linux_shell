#include "apue.h"        // 引入APUE库的头文件,提供了一系列系统编程的工具和函数
#include <netdb.h>       // 包含了网络数据库操作的定义,如主机名解析等
#include <errno.h>       // 定义了错误码,用于错误处理
#include <syslog.h>      // 提供了系统日志的功能
#include <sys/socket.h>  // 包含了套接字编程所需的定义

#define BUFLEN 128  // 定义了缓冲区的大小为128字节
#define QLEN   10   // 定义了队列的最大长度为10

#ifndef HOST_NAME_MAX      // 如果HOST_NAME_MAX未定义,则定义它
#define HOST_NAME_MAX 256  // 主机名的最大长度为256字节
#endif

// 初始化服务器,创建监听套接字并绑定地址
// 参数:int port - 监听端口;const struct sockaddr *addr - 地址结构体指针;socklen_t addrlen - 地址长度;int backlog - 连接队列长度
extern int initserver(int, const struct sockaddr *, socklen_t, int);

// 服务函数,处理客户端连接请求
void serve(int sockfd)
{
    int   clfd;         // 客户端文件描述符
    FILE *fp;           // 文件指针,用于执行系统命令
    char  buf[BUFLEN];  // 缓冲区,用于存储数据

    set_cloexec(sockfd);  // 设置文件描述符为关闭执行时关闭
    for (;;)
    {
        // 接受客户端连接请求
        if ((clfd = accept(sockfd, NULL, NULL)) < 0)
        {
            syslog(LOG_ERR, "ruptimed: accept error: %s", strerror(errno));  // 记录错误日志
            exit(1);                                                         // 退出程序
        }
        set_cloexec(clfd);                                 // 设置客户端文件描述符为关闭执行时关闭
        if ((fp = popen("/usr/bin/uptime", "r")) == NULL)  // 执行系统命令获取系统运行时间
        {
            sprintf(buf, "error: %s\n", strerror(errno));  // 格式化错误信息
            send(clfd, buf, strlen(buf), 0);               // 发送错误信息给客户端
        }
        else
        {
            // 读取系统运行时间并发送给客户端
            // 从文件中读取数据,并将每一行数据发送到客户端套接字
            // 使用fgets函数从文件指针fp指向的文件中读取数据到buf缓冲区,最多读取BUFLEN-1个字符
            // 如果读取成功,fgets返回buf的指针;如果遇到文件结束或读取错误,则返回NULL
            while (fgets(buf, BUFLEN, fp) != NULL)
                // 使用send函数将buf中的数据发送到客户端套接字clfd
                // strlen(buf)计算buf中字符串的长度,第三个参数0表示发送数据时不设置任何标志
                send(clfd, buf, strlen(buf), 0);

            pclose(fp);  // 关闭文件指针
        }
        close(clfd);  // 关闭客户端连接
    }
}
int main(int argc, char *argv[])
{
    // 定义地址信息结构体指针
    struct addrinfo *ailist, *aip;
    // 定义地址信息提示结构体
    struct addrinfo hint;
    // 定义套接字文件描述符,错误码和主机名长度
    int sockfd, err, n;
    // 定义指向主机名的指针
    char *host;

    // 检查命令行参数数量,如果不是1个则打印使用方法并退出
    if (argc != 1)
        err_quit("usage: ruptimed");
    // 获取主机名最大长度,如果失败则使用默认值HOST_NAME_MAX
    if ((n = sysconf(_SC_HOST_NAME_MAX)) < 0)
        n = HOST_NAME_MAX; /* best guess */
    // 分配内存空间用于存储主机名
    if ((host = malloc(n)) == NULL)
        err_sys("malloc error");
    // 获取本地主机名
    if (gethostname(host, n) < 0)
        err_sys("gethostname error");
    // 将程序转为守护进程运行
    daemonize("ruptimed");
    // 初始化地址信息提示结构体
    memset(&hint, 0, sizeof(hint));
    // 设置地址信息提示结构体的标志位,请求返回规范主机名
    hint.ai_flags = AI_CANONNAME;
    // 设置套接字类型为流式套接字,用于TCP连接
    hint.ai_socktype = SOCK_STREAM;
    // 初始化规范主机名字段为空
    hint.ai_canonname = NULL;
    // 初始化地址字段为空
    hint.ai_addr = NULL;
    // 初始化链表的下一个节点为空
    hint.ai_next = NULL;

    // 获取地址信息
    // 获取主机名对应的地址信息
    if ((err = getaddrinfo(host, "ruptime", &hint, &ailist)) != 0)
    {
        /**
         * 如果获取地址信息失败,则记录错误日志并退出程序.
         *
         * @param err      getaddrinfo函数返回的错误码
         * @param host     需要解析的主机名
         * @param hint     getaddrinfo函数的提示结构体
         * @param ailist   存储解析结果的链表
         */
        syslog(LOG_ERR, "ruptimed: getaddrinfo error: %s", gai_strerror(err));  // 记录错误日志
        exit(1);                                                                // 退出程序
    }

    // 遍历地址信息链表
    for (aip = ailist; aip != NULL; aip = aip->ai_next)
    {
        // 初始化服务器套接字
        // 尝试初始化服务器,使用SOCK_STREAM类型的套接字,绑定到aip->ai_addr地址,监听队列长度为QLEN
        if ((sockfd = initserver(SOCK_STREAM, aip->ai_addr, aip->ai_addrlen, QLEN)) >= 0)
        {
            // 如果初始化成功,sockfd将是一个有效的文件描述符
            // 开始服务,处理客户端请求
            serve(sockfd);
            // 服务完成后退出程序
            exit(0);
        }
    }
    // 如果没有成功初始化服务器套接字,则退出程序
    exit(1);
}
