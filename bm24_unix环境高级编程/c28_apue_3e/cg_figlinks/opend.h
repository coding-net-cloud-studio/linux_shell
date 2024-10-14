#include "apue.h"   // 引入APUE库的头文件,该库提供了一套用于编写可移植UNIX应用程序的函数.
#include <errno.h>  // 引入标准错误处理头文件,定义了错误码及其宏.

// 定义服务器套接字的 well-known 名称
#define CS_OPEN "/tmp/opend.socket"

// 定义客户端请求服务器打开文件的命令
#define CL_OPEN "open"

// 如果 debug 非零,则表示处于交互模式(非守护进程模式)
extern int debug;

// 存储返回给客户端的错误信息字符串
extern char errmsg[];

// 存储打开文件时的标志位,如 O_RDONLY, O_WRONLY 等
extern int oflag;

// 存储客户端请求打开的文件路径
extern char *pathname;

// 定义一个结构体Client,每个连接的客户端对应一个Client结构体
typedef struct
{
    int   fd;   // 文件描述符,如果可用则为-1
    uid_t uid;  // 用户ID
} Client;

// client是一个指向动态分配数组的指针
extern Client *client;
// client_size是client数组中的条目数
extern int client_size;

// cli_args 函数用于处理命令行参数
// 参数: argc - 命令行参数的数量
//       argv - 命令行参数的数组
// 返回值: 整数,表示处理结果
int cli_args(int argc, char **argv);

// client_add 函数用于添加一个新的客户端
// 参数: fd - 客户端的文件描述符
//       uid - 客户端的用户ID
// 返回值: 整数,表示添加结果
int client_add(int fd, uid_t uid);

// client_del 函数用于删除一个客户端
// 参数: fd - 客户端的文件描述符
// 返回值: 无
void client_del(int fd);

// loop 函数是主事件循环,用于处理客户端请求
// 参数: 无
// 返回值: 无
void loop(void);

// handle_request 函数用于处理客户端的请求
// 参数: data - 请求的数据
//       len - 数据的长度
//       fd - 客户端的文件描述符
//       uid - 客户端的用户ID
// 返回值: 无
void handle_request(char *data, int len, int fd, uid_t uid);
