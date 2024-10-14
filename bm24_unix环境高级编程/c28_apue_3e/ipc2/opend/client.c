// 引入自定义头文件 opend.h,该头文件可能包含了客户端程序所需的函数声明和数据结构定义.
#include "opend.h"

#define NALLOC 10 /* 定义每次分配的客户端结构体数量 */

static void
client_alloc(void) /* 分配更多客户端数组条目 */
{
    int i;

    if (client == NULL) /* 如果客户端数组为空,则首次分配内存 */
        client = malloc(NALLOC * sizeof(Client));
    else /* 如果客户端数组已存在,则重新分配更多内存 */
        client = realloc(client, (client_size + NALLOC) * sizeof(Client));
    if (client == NULL) /* 如果分配失败,打印错误信息并退出 */
        err_sys("can't alloc for client array");

    /* 初始化新分配的条目 */
    // 以下代码段用于初始化客户端数组中的文件描述符
    // 将文件描述符设置为-1表示该条目当前可用,未被占用
    for (i = client_size; i < client_size + NALLOC; i++)
    {
        client[i].fd = -1;  // 设置文件描述符为-1表示该条目可用
    }

    client_size += NALLOC; /* 更新客户端数组的大小 */
}

/*
 * 当有新的客户端连接请求时,由loop()函数调用.
 * @param fd 新客户端的文件描述符
 * @param uid 新客户端的用户ID
 * @return 返回客户端数组中的索引,如果客户端数组已满,则重新分配内存并再次尝试
 */
int client_add(int fd, uid_t uid)
{
    int i;

    // 如果是第一次调用,分配客户端数组内存
    // 检查客户端是否为空,如果为空,则分配一个新的客户端
    if (client == NULL)
        // 调用client_alloc函数来分配一个新的客户端实例
        client_alloc();

again:
    // 在客户端数组中查找一个可用的条目,并为其分配文件描述符和用户ID
    for (i = 0; i < client_size; i++)  // 遍历客户端数组
    {
        if (client[i].fd == -1)  // 如果找到一个条目的文件描述符为-1,表示该条目可用
        {
            /* find an available entry */
            client[i].fd  = fd;   // 分配文件描述符
            client[i].uid = uid;  // 分配用户ID
            return (i);           // 返回客户端数组中的索引
        }
    }

    /* 客户端数组已满,需要重新分配内存以容纳更多客户端 */
    client_alloc();
    goto again; /* 并再次搜索(这次会成功) */
}

/*
 * 当我们完成对一个客户端的服务时,由loop()函数调用此函数.
 *
 * @param fd 客户端的文件描述符
 */
void client_del(int fd)
{
    int i;

    // 遍历客户端数组,寻找匹配的文件描述符
    for (i = 0; i < client_size; i++)
    {
        if (client[i].fd == fd)
        {
            // 找到匹配的客户端后,将其文件描述符设置为-1,表示该位置不再被占用
            client[i].fd = -1;
            return;  // 完成操作后返回
        }
    }
    // 如果没有找到匹配的客户端,记录错误日志
    log_quit("can't find client entry for fd %d", fd);
}
