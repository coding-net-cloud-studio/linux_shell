#include "opend.h"  // 包含自定义头文件,可能包含函数声明和宏定义
#include <poll.h>   // 包含poll函数的头文件,用于I/O多路复用

#define NALLOC 10  // 定义分配/重新分配pollfd结构体的数量

// 函数功能:扩展pollfd结构体数组的大小
// 参数:
// pfd: 指向pollfd结构体数组的指针
// maxfd: 当前pollfd数组的最大文件描述符加1
// 返回值:成功返回扩展后的pollfd结构体数组指针,失败返回NULL
static struct pollfd *
grow_pollfd(struct pollfd *pfd, int *maxfd)
{
    int i;
    int oldmax = *maxfd;           // 保存旧的maxfd值
    int newmax = oldmax + NALLOC;  // 计算新的maxfd值

    // 使用realloc函数重新分配内存空间
    if ((pfd = realloc(pfd, newmax * sizeof(struct pollfd))) == NULL)
        err_sys("realloc error");  // 如果分配失败,输出错误信息并返回NULL

    // 初始化新增加的pollfd结构体
    for (i = oldmax; i < newmax; i++)
    {
        pfd[i].fd      = -1;      // 文件描述符初始化为-1
        pfd[i].events  = POLLIN;  // 监听读事件
        pfd[i].revents = 0;       // 初始化返回事件为0
    }
    *maxfd = newmax;  // 更新maxfd值
    return (pfd);     // 返回扩展后的pollfd结构体数组指针
}

void loop(void)
{
    // 定义整型变量i,listenfd,clifd,nread,numfd和maxfd
    // listenfd用于监听连接请求,clifd用于存储客户端连接的文件描述符
    // nread用于存储从文件描述符读取的字节数
    // numfd表示当前文件描述符的数量
    // maxfd表示当前分配的最大文件描述符数量
    int            i, listenfd, clifd, nread;
    char           buf[MAXLINE];  // 定义字符数组buf,用于存储读取的数据
    uid_t          uid;           // 定义uid_t类型的变量uid,用于存储用户ID
    struct pollfd *pollfd;        // 定义指向pollfd结构体的指针pollfd,用于I/O多路复用

    // 初始化numfd为1,maxfd为NALLOC
    int numfd = 1;
    int maxfd = NALLOC;

    // 分配内存空间以存储NALLOC个pollfd结构体
    // 如果分配失败,调用err_sys函数报告错误
    if ((pollfd = malloc(NALLOC * sizeof(struct pollfd))) == NULL)
        err_sys("malloc error");

    // 初始化pollfd数组,用于存储文件描述符和事件
    for (i = 0; i < NALLOC; i++)
    {
        // 设置文件描述符为-1,表示尚未分配有效的文件描述符
        pollfd[i].fd = -1;
        // 设置感兴趣的事件为POLLIN,即当文件可读时触发事件
        pollfd[i].events = POLLIN;
        // 初始化revents为0,表示尚未发生任何事件
        pollfd[i].revents = 0;
    }

    /* 获取用于监听客户端请求的文件描述符 */
    if ((listenfd = serv_listen(CS_OPEN)) < 0)  // 如果serv_listen函数返回的文件描述符小于0,表示出错
        log_sys("serv_listen error");           // 记录系统错误日志
    client_add(listenfd, 0);                    // 将监听文件描述符添加到客户端列表中,这里使用0作为标识
    pollfd[0].fd = listenfd;                    // 初始化pollfd结构体数组的第一个元素的文件描述符为监听文件描述符

    for (;;)
    {
        // 使用poll函数等待文件描述符数组中的任何一个准备好进行I/O操作
        // pollfd: 文件描述符数组
        // numfd: 数组中文件描述符的数量
        // -1: 指定无限期等待,直到至少有一个文件描述符准备好
        if (poll(pollfd, numfd, -1) < 0)
            // 如果poll函数返回值小于0,表示发生错误,调用log_sys函数记录系统错误日志
            log_sys("poll error");

        // 检查pollfd数组中的第一个文件描述符是否有可读事件
        if (pollfd[0].revents & POLLIN)
        {
            /* 接受新的客户端请求 */
            // 如果接受新连接失败,记录错误日志
            if ((clifd = serv_accept(listenfd, &uid)) < 0)
                log_sys("serv_accept error: %d", clifd);
            // 将新连接的文件描述符和用户ID添加到客户端列表
            client_add(clifd, uid);

            /* 可能需要增加pollfd数组的大小 */
            // 如果当前文件描述符数量等于最大文件描述符数量
            if (numfd == maxfd)
                // 调整pollfd数组大小
                pollfd = grow_pollfd(pollfd, &maxfd);
            // 设置新连接的文件描述符和事件类型
            // 初始化pollfd结构体数组中的元素
            // clifd: 客户端文件描述符
            // numfd: 当前文件描述符在数组中的索引
            // POLLIN: 表示对该文件描述符感兴趣的事件是可读事件
            // revents: 实际发生的事件,在poll调用返回后由内核设置
            pollfd[numfd].fd      = clifd;   // 设置文件描述符
            pollfd[numfd].events  = POLLIN;  // 设置感兴趣的事件为可读
            pollfd[numfd].revents = 0;       // 初始化实际发生的事件为0

            // 增加文件描述符计数
            numfd++;
            // 记录新连接的信息
            log_msg("new connection: uid %d, fd %d", uid, clifd);
        }

        // 遍历文件描述符数组,从索引1开始,直到numfd(文件描述符的数量)
        for (i = 1; i < numfd; i++)
        {
            // 检查pollfd数组中第i个元素的revents字段是否包含POLLHUP标志
            // 如果包含,表示对应的文件描述符已经挂起,执行hungup标签处的代码
            if (pollfd[i].revents & POLLHUP)
            {
                goto hungup;
            }

            else if (pollfd[i].revents & POLLIN)
            {
                /* 从客户端读取参数缓冲区 */
                // 尝试从文件描述符 pollfd[i].fd 读取数据到缓冲区 buf 中,最多读取 MAXLINE 字节
                if ((nread = read(pollfd[i].fd, buf, MAXLINE)) < 0)
                {
                    // 如果读取失败(返回值小于0),记录系统错误日志
                    log_sys("read error on fd %d", pollfd[i].fd);
                }
                else if (nread == 0)
                // 如果读取到的字节数为0,通常表示连接已关闭

                {
                hungup:
                    /* 客户端关闭了连接 */
                    // 记录日志,显示关闭的客户端用户ID和文件描述符
                    // 打印日志信息,表明某个客户端已关闭连接
                    // 参数:uid - 用户ID,fd - 文件描述符
                    log_msg("closed: uid %d, fd %d",
                            client[i].uid,
                            pollfd[i].fd);

                    // 删除与该文件描述符关联的客户端信息
                    // 参数:fd - 文件描述符
                    client_del(pollfd[i].fd);

                    // 关闭指定的文件描述符
                    // 参数:fd - 文件描述符
                    close(pollfd[i].fd);

                    if (i < (numfd - 1))
                    {
                        /* 压缩数组 */
                        // 将最后一个文件描述符的信息移动到当前位置
                        // 将pollfd数组中最后一个元素的文件描述符,事件和返回事件复制到当前索引i的位置
                        // 这样做可能是为了重用数组空间或者进行某种形式的数据交换
                        pollfd[i].fd      = pollfd[numfd - 1].fd;       // 复制文件描述符
                        pollfd[i].events  = pollfd[numfd - 1].events;   // 复制事件
                        pollfd[i].revents = pollfd[numfd - 1].revents;  // 复制返回事件

                        i--; /* 重新检查这个条目 */
                    }
                    // 减少文件描述符的数量
                    numfd--;
                }
                // 处理客户端的请求
                // 当接收到客户端的请求时,调用 handle_request 函数来处理请求
                // 参数 buf 是接收到的数据缓冲区
                // 参数 nread 是读取到的数据字节数
                // 参数 pollfd[i].fd 是客户端的文件描述符
                // 参数 client[i].uid 是客户端的用户ID
                else
                { /* process client's request */
                    // 处理客户端请求的函数调用
                    // 参数buf: 用于存储读取的数据的缓冲区
                    // 参数nread: 实际读取的字节数
                    // 参数pollfd[i].fd: 当前处理的文件描述符
                    // 参数client[i].uid: 当前连接的客户端的用户ID
                    handle_request(buf, nread, pollfd[i].fd, client[i].uid);
                }
            }
        }
    }
}
