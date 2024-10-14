#include "apue.h"   // 包含APUE库的头文件,提供了一系列系统编程的工具和函数
#include <errno.h>  // 包含errno宏定义的头文件,用于报告系统调用的错误代码

// 定义服务器套接字的 well-known 名称
#define CS_OPEN "/tmp/opend.socket"
// 定义客户端请求服务器打开文件的命令
#define CL_OPEN "open"

// 如果 debug 非零,则表示处于交互模式(非守护进程)
extern int debug;
// 错误消息字符串,用于返回给客户端
extern char errmsg[];
// 打开文件的标志,如 O_xxx ...
extern int oflag;
// 客户端请求打开的文件路径
extern char *pathname;

// 每个连接的客户端对应一个 Client 结构体
typedef struct
{
    int   fd;   // 文件描述符,如果可用则为 -1
    uid_t uid;  // 用户 ID
} Client;

// 指向 malloc 分配的客户端数组的指针
extern Client *client;
// 客户端数组中的条目数量
extern int client_size;

/**
 * 解析客户端命令行参数
 * @param argc 命令行参数的数量
 * @param argv 命令行参数的数组
 * @return 返回处理结果
 */
int cli_args(int argc, char **argv);

/**
 * 添加一个新的客户端到客户端数组
 * @param fd 客户端的文件描述符
 * @param uid 客户端的用户 ID
 * @return 返回添加结果
 */
int client_add(int fd, uid_t uid);

/**
 * 从客户端数组中删除一个客户端
 * @param fd 要删除的客户端的文件描述符
 */
void client_del(int fd);

/**
 * 主事件循环
 */
void loop(void);

/**
 * 处理客户端请求
 * @param buf 接收到的请求数据缓冲区
 * @param fd 客户端的文件描述符
 * @param uid 客户端的用户 ID
 */
void	 handle_request(char *, int, int, uid_t);

