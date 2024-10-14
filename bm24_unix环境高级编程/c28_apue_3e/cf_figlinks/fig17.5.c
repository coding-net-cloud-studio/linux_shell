#include "apue.h"        // 引入APUE库的头文件,提供了一系列系统编程的工具函数和宏定义
#include <sys/socket.h>  // 引入套接字编程所需的头文件,包含了创建和管理网络套接字的函数原型和结构体定义
#include <sys/un.h>      // 引入UNIX域套接字编程所需的头文件,包含了UNIX域套接字的结构体定义和相关常量

int main(void)
{
    int                fd, size;  // 定义文件描述符fd和地址结构大小size
    struct sockaddr_un un;        // 定义一个sockaddr_un结构体变量un,用于存储UNIX域套接字的地址

    un.sun_family = AF_UNIX;            // 设置地址族为AF_UNIX
    strcpy(un.sun_path, "foo.socket");  // 将套接字文件路径复制到un结构体中

    // 创建一个UNIX域套接字,如果失败则调用err_sys函数输出错误信息
    // 创建一个UNIX域套接字,用于本地进程间通信
    // AF_UNIX: 指定使用UNIX域协议
    // SOCK_STREAM: 指定套接字类型为流式套接字,提供可靠的,双向的,基于连接的字节流
    // 0: 使用默认协议
    if ((fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
        // 如果socket创建失败,调用err_sys函数输出错误信息并退出程序
        err_sys("socket failed");

    // 计算地址结构的大小,包括sun_path字段的长度
    // 计算UNIX域套接字地址结构的大小
    // offsetof(struct sockaddr_un, sun_path) 获取结构体中sun_path字段的偏移量
    // strlen(un.sun_path) 获取sun_path字符串的长度
    // 两者相加得到整个结构体的大小,以便正确分配内存
    size = offsetof(struct sockaddr_un, sun_path) + strlen(un.sun_path);

    // 将套接字绑定到指定的地址,如果失败则调用err_sys函数输出错误信息

    // 绑定套接字到指定的地址和端口
    // 参数fd是套接字描述符,un是指向sockaddr结构体的指针,包含了要绑定的地址信息,size是sockaddr结构体的大小
    // 如果绑定失败,返回-1,并通过err_sys函数输出错误信息
    if (bind(fd, (struct sockaddr *)&un, size) < 0)
        err_sys("bind failed");

    printf("UNIX domain socket bound\n");  // 输出绑定成功的消息
    exit(0);                               // 正常退出程序
}
