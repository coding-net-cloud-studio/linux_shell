#include "apue.h"

// 注册退出时的处理函数,如果注册失败则输出错误信息
static void my_exit1(void);
static void my_exit2(void);

int main(void)
{
    // 注册 my_exit2 函数为程序退出时的处理函数,如果注册失败则输出错误信息
    if (atexit(my_exit2) != 0)
        err_sys("can't register my_exit2");

    // 注册 my_exit1 函数为程序退出时的处理函数,如果注册失败则输出错误信息
    if (atexit(my_exit1) != 0)
        err_sys("can't register my_exit1");

    // 再次注册 my_exit1 函数,演示多次注册同一个退出处理函数的效果,如果注册失败则输出错误信息
    if (atexit(my_exit1) != 0)
        err_sys("can't register my_exit1");

    // 主函数执行完毕的输出信息
    printf("main is done\n");
    // 返回0表示程序正常退出
    return (0);
}

// 第一个退出处理函数,输出信息
static void my_exit1(void)
{
    printf("first exit handler\n");
}

// 第二个退出处理函数,输出信息
static void my_exit2(void)
{
    printf("second exit handler\n");
}
