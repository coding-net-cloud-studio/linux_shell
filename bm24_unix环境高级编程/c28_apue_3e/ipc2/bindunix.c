#include "apue.h"        // 引入APUE库的头文件
#include <sys/socket.h>  // 引入系统socket接口的头文件
#include <sys/un.h>      // 引入UNIX域套接字相关的头文件

int main(void)
{
    int                fd, size;  // 定义文件描述符fd和地址结构大小size
    struct sockaddr_un un;        // 定义UNIX域套接字地址结构

    un.sun_family = AF_UNIX;            // 设置地址族为UNIX域
    strcpy(un.sun_path, "foo.socket");  // 设置套接字路径
    // 创建UNIX域流套接字,失败则调用err_sys输出错误信息
    if ((fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
        err_sys("socket failed");
    // 计算地址结构的大小
    // 计算UNIX域套接字地址结构的大小
    // offsetof(struct sockaddr_un, sun_path) 获取sun_path字段在结构体中的偏移量
    // strlen(un.sun_path) 获取sun_path字符串的长度
    // 两者相加得到整个结构体所需的大小
    size = offsetof(struct sockaddr_un, sun_path) + strlen(un.sun_path);

    // 绑定套接字到地址,失败则调用err_sys输出错误信息

    // 绑定套接字到UNIX域地址
    // 参数fd是套接字描述符
    // 参数un是UNIX域地址结构体指针
    // 参数size是地址结构体的大小
    // 如果绑定失败,调用err_sys函数输出错误信息并退出程序
    if (bind(fd, (struct sockaddr *)&un, size) < 0)
        err_sys("bind failed");

    printf("UNIX domain socket bound\n");  // 绑定成功,输出提示信息
    exit(0);                               // 退出程序
}
