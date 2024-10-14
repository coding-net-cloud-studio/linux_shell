#include "apue.h"

// 定义第一个退出处理函数
static void my_exit1(void);
// 定义第二个退出处理函数
static void my_exit2(void);

int main(void)
{
    // 注册第二个退出处理函数,如果注册失败则输出错误信息
    if (atexit(my_exit2) != 0)
        err_sys("can't register my_exit2");

    // 注册第一个退出处理函数,如果注册失败则输出错误信息
    if (atexit(my_exit1) != 0)
        err_sys("can't register my_exit1");
    // 再次注册第一个退出处理函数,演示可以注册多个相同的退出处理函数
    if (atexit(my_exit1) != 0)
        err_sys("can't register my_exit1");

    // 主函数执行完毕
    printf("main is done\n");
    // 返回0表示程序正常退出
    return (0);
}

// 第一个退出处理函数的实现
static void
my_exit1(void)
{
    printf("first exit handler\n");
}

// 第二个退出处理函数的实现
static void
my_exit2(void)
{
    printf("second exit handler\n");
}
