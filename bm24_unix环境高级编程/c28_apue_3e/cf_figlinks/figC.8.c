// 引入 apue.h 头文件,该文件可能包含了程序所需的系统调用和库函数的声明
#include "apue.h"

// 声明两个静态函数 f1 和 f2,这两个函数将在 main 函数中被调用
static void f1(void), f2(void);

// main 函数是程序的入口点
int main(void)
{
    // 调用 f1 函数
    f1();
    // 调用 f2 函数
    f2();
    // 使用 _exit 函数立即终止当前进程,返回状态码 0
    _exit(0);
}

// f1 函数定义,该函数用于创建一个新的进程
static void
f1(void)
{
    pid_t pid;

    // 使用 vfork 函数创建一个子进程,如果返回值小于 0,表示创建失败
    if ((pid = vfork()) < 0)
        // 调用 err_sys 函数输出错误信息并终止程序
        err_sys("vfork error");
    /* child and parent both return */
}

// f2 函数定义,该函数用于初始化一个字符数组
static void
f2(void)
{
    // 定义一个大小为 1000 的字符数组 buf,用于存储数据
    char buf[1000];
    // 定义一个整数变量 i,用于循环计数
    int i;

    // 使用 for 循环将 buf 数组的每个元素初始化为 0
    for (i = 0; i < sizeof(buf); i++)
        buf[i] = 0;
}
