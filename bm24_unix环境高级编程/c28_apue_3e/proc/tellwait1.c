#include "apue.h"

// 子进程和父进程交替输出字符的函数
// 参数:str - 要输出的字符串
static void charatatime(char *str);

int main(void)
{
    pid_t pid;  // 进程ID

    // 创建子进程
    if ((pid = fork()) < 0)
    {
        err_sys("fork error");  // 如果出错,打印错误信息
    }
    else if (pid == 0)  // 子进程执行的代码
    {
        charatatime("output from child\n");  // 输出子进程的信息
    }
    else  // 父进程执行的代码
    {
        charatatime("output from parent\n");  // 输出父进程的信息
    }
    exit(0);  // 退出进程
}

// 逐字符输出字符串的函数
// 参数:str - 要输出的字符串
static void
charatatime(char *str)
{
    char *ptr;  // 指向当前字符的指针
    int   c;    // 当前字符

    setbuf(stdout, NULL);                // 设置标准输出为无缓冲模式
    for (ptr = str; (c = *ptr++) != 0;)  // 遍历字符串直到遇到空字符
        putc(c, stdout);                 // 输出当前字符
}
