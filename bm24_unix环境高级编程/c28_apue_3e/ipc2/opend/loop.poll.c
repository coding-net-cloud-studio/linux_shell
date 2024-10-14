#include "opend.h"
#include <poll.h>

// NALLOC 定义了每次分配或重新分配的 pollfd 结构体的数量.
// 这个宏用于控制内存分配的粒度,以便在轮询多个文件描述符时管理资源.
#define NALLOC 10 /* # pollfd structs to alloc/realloc */

static struct pollfd *
grow_pollfd(struct pollfd *pfd, int *maxfd)  // 扩展pollfd数组的函数,用于处理更多的文件描述符
{
    int i;
    int oldmax = *maxfd;           // 保存旧的文件描述符最大值
    int newmax = oldmax + NALLOC;  // 计算新的文件描述符最大值

    // 使用realloc重新分配内存空间,如果失败则输出错误信息并退出
    if ((pfd = realloc(pfd, newmax * sizeof(struct pollfd))) == NULL)
        err_sys("realloc error");

    // 初始化新增加的pollfd结构体
    for (i = oldmax; i < newmax; i++)
    {
        pfd[i].fd      = -1;      // 文件描述符初始化为-1,表示未使用
        pfd[i].events  = POLLIN;  // 默认监听可读事件
        pfd[i].revents = 0;       // 初始化实际发生的事件为0
    }

    *maxfd = newmax;  // 更新文件描述符最大值
    return (pfd);     // 返回扩展后的pollfd数组
}

void loop(void)
{

    // 定义整型变量i,listenfd,clifd,nread,numfd,maxfd
    int i, listenfd, clifd, nread;
    // 定义字符数组buf,用于存储读取的数据
    char buf[MAXLINE];
    // 定义uid_t类型的变量uid,用于存储用户ID
    uid_t uid;
    // 定义pollfd结构体指针pollfd,用于存储文件描述符的事件
    struct pollfd *pollfd;
    // 定义整型变量numfd,初始化为1,表示当前文件描述符的数量
    int numfd = 1;
    // 定义整型变量maxfd,初始化为NALLOC,表示当前分配的最大文件描述符数量
    int maxfd = NALLOC;

    // 为pollfd数组分配内存,每个元素都是一个struct pollfd结构体
    if ((pollfd = malloc(NALLOC * sizeof(struct pollfd))) == NULL)
        err_sys("malloc error");  // 如果分配失败,打印错误信息并退出

    // 初始化pollfd数组,设置每个文件描述符的事件类型为POLLIN(可读)
    for (i = 0; i < NALLOC; i++)
    {
        pollfd[i].fd      = -1;      // 初始化文件描述符为-1
        pollfd[i].events  = POLLIN;  // 设置感兴趣的事件为可读
        pollfd[i].revents = 0;       // 初始化实际发生的事件为0
    }

    // 获取监听客户端请求的文件描述符
    if ((listenfd = serv_listen(CS_OPEN)) < 0)
        log_sys("serv_listen error");  // 如果监听失败,记录错误信息

    client_add(listenfd, 0);  // 将监听文件描述符添加到客户端列表中
    pollfd[0].fd = listenfd;  // 将监听文件描述符赋值给pollfd数组的第一个元素

    for (;;)
    {
        // 使用poll函数等待文件描述符上的事件,超时时间设置为-1表示无限等待
        if (poll(pollfd, numfd, -1) < 0)
            log_sys("poll error");  // 如果poll函数返回值小于0,表示发生错误,记录系统日志

        // 检查第一个文件描述符是否有数据可读
        if (pollfd[0].revents & POLLIN)
        {
            // 接受新的客户端请求
            if ((clifd = serv_accept(listenfd, &uid)) < 0)
                log_sys("serv_accept error: %d", clifd);  // 如果接受请求失败,记录系统日志
            client_add(clifd, uid);                       // 将新的客户端文件描述符和用户ID添加到客户端列表

            // 如果当前文件描述符数量等于最大文件描述符数量,可能需要增加pollfd数组的大小
            if (numfd == maxfd)
                pollfd = grow_pollfd(pollfd, &maxfd);  // 调整pollfd数组大小

            // 将新的客户端文件描述符添加到pollfd数组中
            pollfd[numfd].fd      = clifd;
            pollfd[numfd].events  = POLLIN;                        // 设置感兴趣的事件为可读
            pollfd[numfd].revents = 0;                             // 重置实际发生的事件
            numfd++;                                               // 增加文件描述符数量
            log_msg("new connection: uid %d, fd %d", uid, clifd);  // 记录新的连接信息
        }

        // 循环检查所有文件描述符的状态
        for (i = 1; i < numfd; i++)
        {
            // 如果文件描述符挂起
            if (pollfd[i].revents & POLLHUP)
            {
                goto hungup;  // 跳转到处理挂起的代码块
            }
            // 如果文件描述符可读
            else if (pollfd[i].revents & POLLIN)
            {
                // 从客户端读取参数缓冲区
                if ((nread = read(pollfd[i].fd, buf, MAXLINE)) < 0)
                {
                    // 记录读取文件描述符fd时发生的错误
                    log_sys("read error on fd %d", pollfd[i].fd);
                }
                else if (nread == 0)
                {
                hungup:
                    // 客户端关闭了连接
                    // 记录客户端关闭连接的信息,包括用户ID和文件描述符
                    log_msg("closed: uid %d, fd %d",
                            client[i].uid,
                            pollfd[i].fd);
                    // 删除客户端信息
                    client_del(pollfd[i].fd);
                    // 关闭文件描述符
                    close(pollfd[i].fd);
                    if (i < (numfd - 1))
                    {
                        // 压缩数组
                        // 将最后一个文件描述符的信息移动到当前位置
                        // 将 pollfd 数组中最后一个元素的 fd,events 和 revents 复制到当前索引 i 的位置
                        // 这样做通常是为了重用数组空间或进行某种形式的数据处理
                        pollfd[i].fd      = pollfd[numfd - 1].fd;       // 复制文件描述符
                        pollfd[i].events  = pollfd[numfd - 1].events;   // 复制感兴趣的事件
                        pollfd[i].revents = pollfd[numfd - 1].revents;  // 复制实际发生的事件

                        i--;  // 重新检查这个条目
                    }
                    numfd--;
                }
                else
                {  // 处理客户端的请求
                    // 处理客户端请求,传入缓冲区数据,读取的字节数,文件描述符和用户ID
                    handle_request(buf, nread, pollfd[i].fd, client[i].uid);
                }
            }
        }
    }
}
