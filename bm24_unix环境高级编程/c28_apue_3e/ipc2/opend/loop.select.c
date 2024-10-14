#include "opend.h"       // 包含自定义头文件,可能包含函数声明和宏定义等
#include <sys/select.h>  // 包含select函数的系统头文件,用于监视多个文件描述符

void loop(void)
{

    // 定义整型变量i, n, maxfd, maxi, listenfd, clifd, nread
    int i, n, maxfd, maxi, listenfd, clifd, nread;

    // 定义字符数组buf,用于存储读取的数据
    char buf[MAXLINE];

    // 定义uid_t类型的变量uid,用于存储用户ID
    uid_t uid;

    // 定义fd_set类型的变量rset和allset,用于存储文件描述符集合
    fd_set rset, allset;

    // 初始化文件描述符集合
    FD_ZERO(&allset);

    // 获取用于监听客户端请求的文件描述符
    // serv_listen函数用于启动服务器监听,CS_OPEN是一个常量,表示以开放模式监听
    // 如果返回值小于0,表示监听失败,记录系统日志
    if ((listenfd = serv_listen(CS_OPEN)) < 0)
        log_sys("serv_listen error");

    // 将监听文件描述符添加到文件描述符集合中
    FD_SET(listenfd, &allset);

    // 初始化最大文件描述符为监听文件描述符
    maxfd = listenfd;

    // 初始化最大索引为-1,用于后续的循环处理
    maxi = -1;

    // 无限循环,用于处理网络事件
    for (;;)
    {
        // rset 初始化为 allset,因为每次循环 rset 都会被修改
        rset = allset;

        // 使用 select 函数监控文件描述符集合,maxfd + 1 是因为 select 函数需要一个比最大文件描述符大 1 的参数
        // 如果返回值小于 0,表示发生了错误
        if ((n = select(maxfd + 1, &rset, NULL, NULL, NULL)) < 0)
            log_sys("select error");  // 记录系统错误

        // 检查监听文件描述符 listenfd 是否在就绪集合 rset 中
        if (FD_ISSET(listenfd, &rset))
        {
            // 接受新的客户端请求
            if ((clifd = serv_accept(listenfd, &uid)) < 0)
                log_sys("serv_accept error: %d", clifd);  // 记录接受客户端连接时的错误

            i = client_add(clifd, uid);  // 将新的客户端添加到客户端数组中
            FD_SET(clifd, &allset);      // 将新的文件描述符添加到 allset 集合中

            // 更新最大文件描述符 maxfd 和客户端数组中的最大索引 maxi
            if (clifd > maxfd)
            {
                maxfd = clifd;  // 如果当前文件描述符大于maxfd,则更新maxfd
            }
            if (i > maxi)
            {
                maxi = i;  // 如果当前索引大于maxi,则更新maxi
            }

            log_msg("new connection: uid %d, fd %d", uid, clifd);  // 记录新的连接信息
            continue;                                              // 继续下一次循环
        }

        // 遍历客户端数组
        for (i = 0; i <= maxi; i++)
        {
            // 检查客户端文件描述符是否有效,无效则跳过当前循环
            if ((clifd = client[i].fd) < 0)
                continue;

            // 检查文件描述符是否在读集合中
            if (FD_ISSET(clifd, &rset))
            {
                // 读取数据,如果出错则记录错误日志
                if ((nread = read(clifd, buf, MAXLINE)) < 0)
                {
                    log_sys("read error on fd %d", clifd);
                }
                // 如果读取的字节数为0,表示客户端已关闭连接
                else if (nread == 0)
                {
                    log_msg("closed: uid %d, fd %d", client[i].uid, clifd);
                    // 删除客户端信息
                    client_del(clifd); /* client has closed cxn */
                    // 从所有集合中清除文件描述符
                    FD_CLR(clifd, &allset);
                    // 关闭文件描述符
                    close(clifd);
                }
                else
                { /* 处理客户端的请求 */
                    // 处理请求,传入读取的数据,字节数,文件描述符和用户ID
                    handle_request(buf, nread, clifd, client[i].uid);
                }
            }
        }
    }
}
