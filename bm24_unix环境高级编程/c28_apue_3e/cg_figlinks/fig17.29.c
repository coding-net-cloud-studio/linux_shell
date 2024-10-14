#include "opend.h"       // 包含自定义头文件,可能包含函数声明和宏定义
#include <sys/select.h>  // 包含select函数的系统头文件,用于I/O多路复用

void loop(void)
{

    // 定义整型变量i, n, maxfd, maxi, listenfd, clifd, nread
    // i: 循环变量
    // n: 用于存储select函数的返回值
    // maxfd: 监视的文件描述符中的最大值加1
    // maxi: 当前最大描述符的索引
    // listenfd: 监听套接字的文件描述符
    // clifd: 客户端连接的文件描述符
    // nread: 从文件描述符读取的字节数
    int i, n, maxfd, maxi, listenfd, clifd, nread;

    // 定义字符数组buf,用于存储从文件描述符读取的数据
    // MAXLINE: buf的最大长度
    char buf[MAXLINE];

    // 定义uid_t类型的变量uid,用于存储用户ID
    uid_t uid;

    // 定义两个fd_set类型的变量rset和allset
    // rset: 当前正在监视的可读文件描述符集合
    // allset: 包含所有文件描述符的集合,用于初始化rset
    fd_set rset, allset;

    // 初始化文件描述符集合
    FD_ZERO(&allset);

    // 获取用于监听客户端请求的文件描述符
    // serv_listen函数用于启动服务器监听,CS_OPEN是一个常量,表示以开放模式监听
    // 如果返回值小于0,表示监听失败,调用log_sys记录系统错误
    if ((listenfd = serv_listen(CS_OPEN)) < 0)
        log_sys("serv_listen error");

    // 将监听文件描述符添加到文件描述符集合中
    FD_SET(listenfd, &allset);

    // 初始化最大文件描述符为监听文件描述符
    maxfd = listenfd;

    // 初始化最大索引为-1,用于后续的文件描述符管理
    maxi = -1;

    // 无限循环,通常用于程序的主事件循环或等待用户输入等情况.
    // 由于这是一个无限循环,没有明确的退出条件,因此需要确保在循环体内有适当的退出机制,
    // 例如检查用户输入,系统信号或其他条件来跳出循环.
    for (;;)
    {
        rset = allset;                                            /* rset获取allset的值,rset在每次循环中都会被修改 */
        if ((n = select(maxfd + 1, &rset, NULL, NULL, NULL)) < 0) /* 使用select函数监视文件描述符集合,maxfd + 1是监视的最大文件描述符加一,&rset是要监视的读集合,NULL表示不监视写和异常集合,NULL表示没有超时时间 */
            log_sys("select error");                              /* 如果select函数返回值小于0,表示发生错误,调用log_sys函数记录错误信息 */

        if (FD_ISSET(listenfd, &rset))  // 检查监听文件描述符是否在读集合中
        {
            /* accept new client request */                        // 接受新的客户端请求
            if ((clifd = serv_accept(listenfd, &uid)) < 0)         // 如果接受请求失败
                log_sys("serv_accept error: %d", clifd);           // 记录系统错误日志
            i = client_add(clifd, uid);                            // 将新连接的客户端添加到客户端数组中
            FD_SET(clifd, &allset);                                // 将新的文件描述符添加到所有文件描述符集合中
            if (clifd > maxfd)                                     // 更新最大文件描述符的值
                maxfd = clifd;                                     // 用于select()的最大文件描述符
            if (i > maxi)                                          // 更新客户端数组中的最大索引值
                maxi = i;                                          // 客户端数组中的最大索引
            log_msg("new connection: uid %d, fd %d", uid, clifd);  // 记录新连接的日志信息
            continue;                                              // 继续下一次循环
        }

        // 遍历从0到maxi的所有整数
        // 注意:循环条件是i <= maxi,这意味着循环将包括maxi在内的所有整数
        for (i = 0; i <= maxi; i++)
        { /* go through client[] array */
            // 检查客户端文件描述符是否有效
            // 如果客户端的文件描述符小于0,则跳过当前循环迭代,继续检查下一个客户端
            if ((clifd = client[i].fd) < 0)
                continue;

            // 检查客户端文件描述符是否准备好进行读取
            if (FD_ISSET(clifd, &rset))
            {
                /* 从客户端读取参数缓冲区 */
                // 尝试从文件描述符 clifd 读取数据到缓冲区 buf,最多读取 MAXLINE 字节
                if ((nread = read(clifd, buf, MAXLINE)) < 0)
                {
                    // 如果读取失败,记录系统错误日志
                    log_sys("read error on fd %d", clifd);
                }
                else if (nread == 0)
                {
                    // 如果读取的字节数为0,表示客户端已关闭连接
                    // 打印日志信息,显示已关闭的客户端连接的用户ID和文件描述符
                    // 参数:
                    //   client[i].uid - 客户端的用户ID
                    //   clifd - 客户端的文件描述符
                    log_msg("closed: uid %d, fd %d",
                            client[i].uid,
                            clifd);

                    // 删除客户端信息
                    client_del(clifd);
                    // 清除文件描述符集合中的 clifd
                    FD_CLR(clifd, &allset);
                    // 关闭文件描述符
                    close(clifd);
                }
                else
                {
                    // 处理客户端的请求
                    // 处理客户端请求的函数调用
                    // 参数buf: 包含请求数据的缓冲区
                    // 参数nread: 缓冲区中读取的字节数
                    // 参数clifd: 客户端的文件描述符
                    // 参数client[i].uid: 当前客户端的用户ID
                    handle_request(buf, nread, clifd, client[i].uid);  // 调用函数处理请求
                }
            }
        }
    }
}
