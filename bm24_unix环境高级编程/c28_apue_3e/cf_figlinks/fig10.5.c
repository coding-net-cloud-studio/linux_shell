#include "apue.h"
#include <pwd.h>

// 信号处理函数,当接收到信号时会被调用
static void
my_alarm(int signo)
{

    // 定义一个指向passwd结构体的指针rootptr
    // passwd结构体通常用于存储用户账户信息
    struct passwd *rootptr;

    // 打印信号处理函数已进入的消息
    printf("in signal handler\n");

    // 获取名为"root"的用户信息
    if ((rootptr = getpwnam("root")) == NULL)
        err_sys("getpwnam(root) error");  // 如果获取失败,打印错误信息并退出程序

    // 设置一个新的闹钟信号,1秒后触发
    alarm(1);
}
// fig10.5.c

// 主函数
int main(void)
{
    // 定义一个指向passwd结构体的指针
    struct passwd *ptr;

    // 设置SIGALRM信号的处理函数为my_alarm
    signal(SIGALRM, my_alarm);
    // 设置一个1秒后触发的闹钟
    alarm(1);
    // 无限循环
    for (;;)
    {
        // 获取用户名为"sar"的用户信息,如果失败则输出错误信息并退出
        // 如果ptr等于通过getpwnam函数获取的用户名为"sar"的用户信息,则ptr为NULL
        // getpwnam函数用于通过用户名获取用户信息,如果找不到对应的用户则返回NULL
        if ((ptr = getpwnam("sar")) == NULL)
            err_sys("getpwnam error");
        // 检查获取到的用户名是否为"sar",如果不是则输出错误信息
        // 检查ptr指向的结构体中的pw_name字段是否不等于字符串"sar"
        // 如果不相等,则打印错误信息,指示返回值已损坏,并显示pw_name的值
        if (strcmp(ptr->pw_name, "sar") != 0)
            printf("return value corrupted!, pw_name = %s\n", ptr->pw_name);
    }
}
