#include "apue.h"   // 引入apue库头文件
#include <errno.h>  // 引入错误处理头文件

// 客户端请求服务器打开文件的命令
#define CL_OPEN "open"

// 错误消息字符串,用于返回给客户端
extern char errmsg[];

// open()函数的标志位:O_xxx等
extern int oflag;

// 客户端请求打开的文件路径
extern char *pathname;

/**
 * cli_args - 处理客户端命令行参数
 * @argc: 命令行参数的数量
 * @argv: 命令行参数的数组
 *
 * 返回值: 成功返回0,失败返回-1
 */
int cli_args(int argc, char **argv);

/**
 * handle_request - 处理客户端请求
 * @msg: 客户端发送的消息
 * @connfd: 客户端连接的文件描述符
 * @pid: 客户端的进程ID
 */
void handle_request(char *msg, int connfd, int pid);
