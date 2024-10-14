#include "apue.h"   // 包含APUE库的头文件
#include <errno.h>  // 包含错误处理头文件

#define CL_OPEN "open"  // 客户端请求服务器打开文件的标识

// 外部变量声明
extern char  errmsg[];  // 用于返回给客户端的错误信息字符串
extern int   oflag;     // open()函数的标志位: O_xxx ...
extern char *pathname;  // 客户端请求打开的文件路径

/**
 * @brief 处理客户端命令行参数
 *
 * @param argc 命令行参数的数量
 * @param argv 命令行参数的数组
 * @return int 返回处理结果
 */
int cli_args(int argc, char **argv);

/**
 * @brief 处理客户端请求
 *
 * @param buf 接收到的请求数据缓冲区
 * @param sockfd 客户端的socket文件描述符
 * @param oflag 打开文件的标志位
 */
void handle_request(char *buf, int sockfd, int oflag);
