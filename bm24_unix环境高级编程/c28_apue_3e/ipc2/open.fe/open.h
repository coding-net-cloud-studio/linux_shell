#include "apue.h"   // 包含APUE库的头文件
#include <errno.h>  // 包含错误处理库的头文件

// 定义客户端请求服务器打开文件的宏
#define CL_OPEN "open"

// 函数声明:客户端打开文件
// 参数:char * - 文件路径;int - 打开模式
// 返回值:int - 文件描述符或错误码
int csopen(char *, int);
