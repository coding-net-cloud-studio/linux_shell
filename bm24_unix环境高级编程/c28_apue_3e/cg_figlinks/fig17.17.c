/*
 * 包含必要的头文件
 */
#include "apue.h"   // 引入apue库,可能包含自定义的系统编程接口和实用函数
#include <errno.h>  // 引入errno头文件,用于访问系统调用的错误代码

/*
 * 定义客户端请求字符串
 */
#define CL_OPEN "open" /* client's request for server */

/*
 * 声明外部函数 csopen,该函数用于与服务器建立连接并打开文件
 */
int csopen(char *, int);
