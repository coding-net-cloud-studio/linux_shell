#include "apue.h"   // 引入apue库,可能包含自定义的系统调用封装和错误处理函数
#include <errno.h>  // 引入标准库中的errno.h,用于访问系统调用的错误代码

// 定义服务器监听的套接字路径
#define CS_OPEN "/tmp/opend.socket"
// 定义客户端请求的字符串
#define CL_OPEN "open"

// 声明外部变量,用于调试模式的标志
extern int debug;
// 声明外部变量,用于存储错误消息的字符串
extern char errmsg[];
// 声明外部变量,用于存储打开文件的标志
extern int oflag;
// 声明外部变量,用于存储要打开的文件名
extern char *pathname;

// 定义一个结构体,用于存储每个连接的客户端的信息
typedef struct
{
    int   fd;   // 文件描述符,或-1表示可用
    uid_t uid;  // 用户ID
} Client;

// 声明外部变量,指向动态分配的客户端数组
extern Client *client;
// 声明外部变量,存储客户端数组的大小
extern int client_size;

// 解析客户端命令行参数的函数
int cli_args(int, char **);
// 添加客户端到客户端数组的函数
int client_add(int, uid_t);
// 从客户端数组中删除客户端的函数
void client_del(int);
// 主循环函数,处理客户端请求
void loop(void);
// 处理客户端请求的函数
void handle_request(char *, int, int, uid_t);
