#include "apue.h"
#include <pwd.h>

// 信号处理函数,当接收到SIGALRM信号时会被调用
static void
my_alarm(int signo)
{
    // 定义一个指向passwd结构体的指针rootptr
    // passwd结构体通常用于存储用户账户信息,包括用户名,密码等
    // 这里rootptr可能被用来指向超级用户(root)的信息
    struct passwd *rootptr;

    printf("in signal handler\n");             // 打印信号处理函数进入的信息
    if ((rootptr = getpwnam("root")) == NULL)  // 尝试获取root用户的信息
        err_sys("getpwnam(root) error");       // 如果失败,打印错误信息
    alarm(1);                                  // 设置一个新的闹钟信号,1秒后触发
}

int main(void)
{
    // 定义一个指向passwd结构体的指针ptr
    // passwd结构体通常用于存储用户账户信息
    struct passwd *ptr;

    signal(SIGALRM, my_alarm);  // 注册SIGALRM信号的处理函数为my_alarm
    alarm(1);                   // 设置一个闹钟信号,1秒后触发
    for (;;)                    // 无限循环
    {
        if ((ptr = getpwnam("sar")) == NULL)                   // 尝试获取sar用户的信息
            err_sys("getpwnam error");                         // 如果失败,打印错误信息
        if (strcmp(ptr->pw_name, "sar") != 0)                  // 检查获取到的用户名是否为sar
            printf("return value corrupted!, pw_name = %s\n",  // 如果不是,打印错误信息
                   ptr->pw_name);
    }
}
