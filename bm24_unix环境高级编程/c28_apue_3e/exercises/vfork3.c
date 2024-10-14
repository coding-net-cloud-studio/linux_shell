#include "apue.h"

// 函数声明
static void f1(void), f2(void);

// 主函数
int main(void)
{
    f1();      // 调用f1函数
    f2();      // 调用f2函数
    _exit(0);  // 程序正常退出
}

// f1函数定义
// 使用vfork创建子进程,子进程和父进程共享内存空间
static void
f1(void)
{
    pid_t pid;  // 定义进程ID变量

    if ((pid = vfork()) < 0)            // 如果vfork失败,返回负值
        err_sys("vfork error");         // 输出错误信息
    /* child and parent both return */  // 子进程和父进程都会从这里返回
}

// f2函数定义
// 分配一个字符数组并初始化为0
static void
f2(void)
{
    char buf[1000];  // 定义一个大小为1000的字符数组(自动变量)
    int  i;          // 定义循环变量

    for (i = 0; i < sizeof(buf); i++)  // 循环填充数组
        buf[i] = 0;                    // 将数组元素设置为0
}
