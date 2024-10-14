#include "opend.h"  // 引入opend.h头文件,可能包含了程序所需的函数声明和定义.

// 定义NALLOC为每次分配或重新分配的客户端结构体数量
#define NALLOC 10

static void
client_alloc(void) /* 分配更多条目在 client[] 数组中 */
{
    // 声明一个整型变量i,但未初始化,其值是不确定的.
    int i;

    // 如果 client 指针为空,则首次分配内存
    // 使用 malloc 函数为 client 分配 NALLOC 个 Client 结构体的空间
    if (client == NULL)
        client = malloc(NALLOC * sizeof(Client));
    else
        // 如果 client 指针不为空,则重新分配内存
        // 使用 realloc 函数增加 NALLOC 个 Client 结构体的空间
        client = realloc(client, (client_size + NALLOC) * sizeof(Client));
    // 检查内存分配是否成功
    // 如果 client 仍为 NULL,则表示内存分配失败,调用 err_sys 函数报告错误
    if (client == NULL)
        err_sys("can't alloc for client array");

    /* 初始化新分配的条目 */
    // 为客户端数组中的每个条目分配空间,并初始化文件描述符为 -1
    // 表示该条目当前可用
    for (i = client_size; i < client_size + NALLOC; i++)
    {
        client[i].fd = -1;  // fd 为 -1 表示条目可用
    }

    // 更新 client 数组的大小
    client_size += NALLOC;
}

/*
 * 当有新客户端的连接请求到达时,由loop()函数调用.
 * @param fd 新客户端的文件描述符
 * @param uid 新客户端的用户ID
 * @return 返回客户端在client[]数组中的索引,如果无法添加则返回错误码
 */
int client_add(int fd, uid_t uid)
{
    int i;

    // 如果是第一次调用client_add,分配客户端数组内存
    if (client == NULL) /* first time we're called */
        client_alloc();

again:
    // 遍历客户端数组,寻找可用的位置
    for (i = 0; i < client_size; i++)
    {
        if (client[i].fd == -1)
        { /* find an available entry */
            // 找到空位,将新客户端的信息存入
            // 为客户端数组中的第i个元素设置文件描述符和用户ID
            client[i].fd  = fd;   // 设置文件描述符
            client[i].uid = uid;  // 设置用户ID

            return (i); /* 返回客户端在数组中的索引 */
        }
    }

    // 如果客户端数组已满,重新分配更大的内存空间
    client_alloc();
    goto again; /* 重新搜索可用位置(这次会成功) */
}

/*
 * 当我们完成对一个客户端的服务时,由loop()函数调用.
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

    // 如果遍历完数组仍未找到匹配的文件描述符,则记录错误日志
    // 日志退出函数,用于记录错误信息并退出程序
    // 参数fd是要查找的客户端条目的文件描述符
    // 如果找不到对应的客户端条目,则记录错误信息并退出程序
    log_quit("can't find client entry for fd %d", fd);
}
