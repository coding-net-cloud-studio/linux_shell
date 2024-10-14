#include "apue.h"        // 引入APUE库头文件,提供系统调用和标准C库函数的封装
#include <netdb.h>       // 包含网络数据库操作的头文件,如主机名解析等
#include <errno.h>       // 定义了错误码,用于错误处理
#include <syslog.h>      // 提供日志记录功能
#include <fcntl.h>       // 包含文件控制操作的头文件,如文件打开模式等
#include <sys/socket.h>  // 包含套接字操作的头文件
#include <sys/wait.h>    // 包含进程等待操作的头文件

#define QLEN 10  // 定义队列长度为10

#ifndef HOST_NAME_MAX      // 如果未定义HOST_NAME_MAX,则定义它
#define HOST_NAME_MAX 256  // 主机名的最大长度为256字节
#endif

// 函数声明:初始化服务器
// 参数:int类型的端口号,指向sockaddr结构体的指针,socklen_t类型的网络地址长度,int类型的标志
// 返回值:int类型,表示初始化是否成功
extern int initserver(int, const struct sockaddr *, socklen_t, int);

void serve(int sockfd)
{
    int   clfd, status;  // clfd: 客户端文件描述符, status: 子进程退出状态
    pid_t pid;           // pid: 进程ID

    set_cloexec(sockfd);  // 设置文件描述符为执行时关闭,防止子进程继承后造成资源泄露
    for (;;)
    {
        // 接受客户端连接请求
        if ((clfd = accept(sockfd, NULL, NULL)) < 0)
        {
            syslog(LOG_ERR, "ruptimed: accept error: %s", strerror(errno));  // 记录错误日志
            exit(1);                                                         // 退出程序
        }
        // 创建子进程处理连接
        if ((pid = fork()) < 0)
        {
            syslog(LOG_ERR, "ruptimed: fork error: %s", strerror(errno));  // 记录错误日志
            exit(1);                                                       // 退出程序
        }
        else if (pid == 0)
        { /* child */
            // 将客户端文件描述符复制到标准输出和标准错误
            // 以下代码段用于将子进程的标准输出(STDOUT_FILENO)和标准错误(STDERR_FILENO)重定向到文件描述符clfd所指向的文件.
            // dup2函数用于复制一个现有的文件描述符,第二个参数指定了新的文件描述符.
            // 如果dup2函数返回值不等于目标文件描述符,说明操作失败.
            if (dup2(clfd, STDOUT_FILENO) != STDOUT_FILENO ||
                dup2(clfd, STDERR_FILENO) != STDERR_FILENO)
            {
                syslog(LOG_ERR, "ruptimed: unexpected error");  // 记录错误日志
                exit(1);                                        // 退出程序
            }
            close(clfd);  // 关闭客户端文件描述符,因为已经复制到标准输出和标准错误
            // 执行uptime命令,将结果输出到客户端
            execl("/usr/bin/uptime", "uptime", (char *)0);
            syslog(LOG_ERR, "ruptimed: unexpected return from exec: %s", strerror(errno));  // 如果execl失败,记录错误日志
        }
        else
        {                              /* parent */
            close(clfd);               // 父进程关闭客户端文件描述符,因为已经交给子进程处理
            waitpid(pid, &status, 0);  // 等待子进程结束
        }
    }
}

int main(int argc, char *argv[])
{
    // 定义地址信息结构体指针和临时变量
    // 定义地址信息结构体指针ailist和aip,用于存储解析主机名后的地址信息
    struct addrinfo *ailist, *aip;

    // 定义addrinfo结构体hint,用于设置地址信息解析的提示
    struct addrinfo hint;

    // 定义sockfd为套接字文件描述符,err为错误码,n为接收或发送的数据字节数
    int sockfd, err, n;

    // 定义host为指向主机名的指针
    char *host;

    // 检查命令行参数,如果不是空则打印使用方法并退出
    if (argc != 1)
        err_quit("usage: ruptimed");

    // 获取主机名的最大长度
    if ((n = sysconf(_SC_HOST_NAME_MAX)) < 0)
        n = HOST_NAME_MAX;  // 如果获取失败则使用默认值

    // 分配内存空间用于存储主机名
    if ((host = malloc(n)) == NULL)
        err_sys("malloc error");

    // 获取本地主机名
    if (gethostname(host, n) < 0)
        err_sys("gethostname error");

    // 将程序转为守护进程运行
    daemonize("ruptimed");

    // 初始化地址信息结构体hint
    memset(&hint, 0, sizeof(hint));
    // 设置地址信息提示结构体的标志位,请求返回规范主机名
    hint.ai_flags = AI_CANONNAME;
    // 设置地址信息提示结构体的套接字类型为流式套接字,用于TCP连接
    hint.ai_socktype = SOCK_STREAM;
    // 初始化地址信息提示结构体的规范主机名字段为空
    hint.ai_canonname = NULL;
    // 初始化地址信息提示结构体的地址字段为空
    hint.ai_addr = NULL;
    // 初始化地址信息提示结构体的下一个地址信息字段为空
    hint.ai_next = NULL;

    // 获取地址信息
    if ((err = getaddrinfo(host, "ruptime", &hint, &ailist)) != 0)
    {
        // 如果获取失败则记录错误日志并退出
        // 使用syslog函数记录错误信息,级别为LOG_ERR,表示严重的错误
        // getaddrinfo函数用于将主机名和服务名映射到一个地址,如果发生错误,err将包含错误码
        // gai_strerror函数将错误码转换为可读的字符串形式
        syslog(LOG_ERR, "ruptimed: getaddrinfo error: %s", gai_strerror(err));  // 记录getaddrinfo函数的错误信息

        exit(1);
    }

    // 遍历地址信息列表,尝试初始化服务器并处理连接
    for (aip = ailist; aip != NULL; aip = aip->ai_next)
    {
        // 初始化服务器
        // 如果成功初始化服务器套接字,则sockfd将是一个有效的文件描述符
        // initserver函数尝试在指定的地址和端口上创建并绑定一个套接字
        // SOCK_STREAM 指定了使用TCP协议
        // aip->ai_addr 是服务器地址结构体的指针,包含了服务器的IP地址和端口信息
        // aip->ai_addrlen 是地址结构体的长度
        // QLEN 是监听队列的最大长度,用于accept函数
        if ((sockfd = initserver(SOCK_STREAM, aip->ai_addr, aip->ai_addrlen, QLEN)) >= 0)
        {
            // 服务处理连接
            serve(sockfd);
            exit(0);  // 成功处理连接后退出
        }
    }

    // 如果没有成功初始化服务器则退出
    exit(1);
}
