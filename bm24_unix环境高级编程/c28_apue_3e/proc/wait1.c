#include "apue.h"
#include <sys/wait.h>

int main(void)
{
    pid_t pid;     // 定义进程ID变量
    int   status;  // 定义状态变量

    // 第一次fork操作
    if ((pid = fork()) < 0)     // 如果fork失败,返回负值
        err_sys("fork error");  // 输出错误信息
    else if (pid == 0)
    { /* child */  // 如果pid为0,表示当前代码在子进程中执行
        exit(7);   // 子进程退出,返回状态码7
    }

    // 等待子进程结束
    if (wait(&status) != pid)   // 如果等待子进程返回的结果不是pid,表示出错
        err_sys("wait error");  // 输出错误信息
    pr_exit(status);            // 打印子进程退出的状态信息

    // 第二次fork操作
    if ((pid = fork()) < 0)     // 如果fork失败,返回负值
        err_sys("fork error");  // 输出错误信息
    else if (pid == 0)
    { /* child */  // 如果pid为0,表示当前代码在子进程中执行
        abort();   // 子进程调用abort函数,生成SIGABRT信号,默认终止进程
    }

    // 等待子进程结束
    if (wait(&status) != pid)   // 如果等待子进程返回的结果不是pid,表示出错
        err_sys("wait error");  // 输出错误信息
    pr_exit(status);            // 打印子进程退出的状态信息

    // 第三次fork操作
    if ((pid = fork()) < 0)     // 如果fork失败,返回负值
        err_sys("fork error");  // 输出错误信息
    else if (pid == 0)
    { /* child */     // 如果pid为0,表示当前代码在子进程中执行
        status /= 0;  // 子进程执行除以0操作,生成SIGFPE信号
    }

    // 等待子进程结束
    if (wait(&status) != pid)   // 如果等待子进程返回的结果不是pid,表示出错
        err_sys("wait error");  // 输出错误信息
    pr_exit(status);            // 打印子进程退出的状态信息

    exit(0);  // 主进程退出,返回状态码0
}
