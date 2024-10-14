#include "apue.h"        // 引入APUE库的头文件
#include <netdb.h>       // 网络数据库操作相关头文件
#include <errno.h>       // 错误号定义头文件
#include <syslog.h>      // 系统日志功能相关头文件
#include <sys/socket.h>  // 套接字编程相关头文件

#define BUFLEN     128  // 定义缓冲区长度
#define MAXADDRLEN 256  // 定义最大地址长度

#ifndef HOST_NAME_MAX      // 如果未定义HOST_NAME_MAX
#define HOST_NAME_MAX 256  // 则定义HOST_NAME_MAX为256
#endif

// 函数声明:初始化服务器
// 参数:int类型的端口号,指向sockaddr结构体的指针,socklen_t类型的地址长度,int类型的选项
// 返回值:int类型,表示初始化是否成功
extern int initserver(int, const struct sockaddr *, socklen_t, int);

void serve(int sockfd)
{
    int              n;                               // 用于存储recvfrom函数返回的字节数
    socklen_t        alen;                            // 用于存储地址长度
    FILE            *fp;                              // 文件指针,用于执行外部命令后的文件流
    char             buf[BUFLEN];                     // 缓冲区,用于存储接收和发送的数据
    char             abuf[MAXADDRLEN];                // 地址缓冲区
    struct sockaddr *addr = (struct sockaddr *)abuf;  // 指向地址结构体的指针

    set_cloexec(sockfd);  // 设置文件描述符为close-on-exec标志

    // 无限循环,等待并处理客户端请求
    for (;;)
    {
        alen = MAXADDRLEN;  // 重置地址长度
        // 接收来自客户端的数据
        if ((n = recvfrom(sockfd, buf, BUFLEN, 0, addr, &alen)) < 0)
        {
            syslog(LOG_ERR, "ruptimed: recvfrom error: %s", strerror(errno));  // 记录错误日志
            exit(1);                                                           // 退出程序
        }
        // 执行uptime命令,获取系统运行时间
        if ((fp = popen("/usr/bin/uptime", "r")) == NULL)
        {
            sprintf(buf, "error: %s\n", strerror(errno));     // 格式化错误信息
            sendto(sockfd, buf, strlen(buf), 0, addr, alen);  // 发送错误信息给客户端
        }
        else
        {
            // 读取uptime命令的输出
            if (fgets(buf, BUFLEN, fp) != NULL)
                sendto(sockfd, buf, strlen(buf), 0, addr, alen);  // 发送输出给客户端
            pclose(fp);                                           // 关闭文件流
        }
    }
}

int main(int argc, char *argv[])
{
    // 定义地址信息结构体指针
    struct addrinfo *ailist, *aip;
    // 定义地址信息结构体
    struct addrinfo hint;
    // 定义套接字描述符,错误码,主机名长度
    int sockfd, err, n;
    // 定义指向主机名的指针
    char *host;

    // 检查命令行参数数量,如果不是1个则打印使用方法并退出
    if (argc != 1)
        err_quit("usage: ruptimed");
    // 获取主机名最大长度,如果出错则使用默认值HOST_NAME_MAX
    if ((n = sysconf(_SC_HOST_NAME_MAX)) < 0)
        n = HOST_NAME_MAX;
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
    // 设置地址信息提示结构体的标志,请求返回规范主机名
    hint.ai_flags = AI_CANONNAME;
    // 设置套接字类型为UDP
    hint.ai_socktype = SOCK_DGRAM;
    // 规范主机名初始化为NULL
    hint.ai_canonname = NULL;
    // 地址信息初始化为NULL
    hint.ai_addr = NULL;
    // 指向下一个地址信息结构体的指针初始化为NULL
    hint.ai_next = NULL;

    // 获取地址信息
    if ((err = getaddrinfo(host, "ruptime", &hint, &ailist)) != 0)
    {
        // 如果出错,记录错误日志并退出
        syslog(LOG_ERR, "ruptimed: getaddrinfo error: %s", gai_strerror(err));
        exit(1);
    }
    // 遍历地址信息链表
    for (aip = ailist; aip != NULL; aip = aip->ai_next)
    {
        // 初始化服务器套接字
        // 尝试初始化UDP服务器,如果成功则返回文件描述符
        if ((sockfd = initserver(SOCK_DGRAM, aip->ai_addr, aip->ai_addrlen, 0)) >= 0)
        {
            // 开始提供服务
            serve(sockfd);
            // 服务结束后退出程序
            exit(0);
        }
    }
    // 如果没有成功初始化服务器套接字,退出程序
    exit(1);
}
