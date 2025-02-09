#include "apue.h"   // 包含APUE库的头文件
#include <errno.h>  // 包含错误处理头文件

// 客户端请求服务器打开文件的命令
#define CL_OPEN "open"
// 服务器众所周知的套接字名称
#define CS_OPEN "/tmp/opend.socket"

/**
 * @brief csopen函数用于客户端请求服务器打开文件
 * @param path 文件路径
 * @param oflag 打开文件的标志
 * @return 成功返回文件描述符,失败返回-1
 */
int csopen(char *, int);
