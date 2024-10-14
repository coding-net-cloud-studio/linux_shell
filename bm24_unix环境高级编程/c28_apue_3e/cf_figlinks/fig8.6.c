#include "apue.h"
#include <sys/wait.h>

int main(void)
{
    pid_t pid;     // 定义进程ID变量
    int   status;  // 定义状态变量

    if ((pid = fork()) < 0)     // 创建子进程
        err_sys("fork error");  // 如果出错,打印错误信息
    else if (pid == 0)          /* child */
        exit(7);                // 子进程退出,返回状态码7

    if (wait(&status) != pid)   // 等待子进程结束
        err_sys("wait error");  // 如果出错,打印错误信息
    pr_exit(status);            // 打印子进程退出状态

    if ((pid = fork()) < 0)     // 再次创建子进程
        err_sys("fork error");  // 如果出错,打印错误信息
    else if (pid == 0)          /* child */
        abort();                // 子进程调用abort函数,生成SIGABRT信号

    if (wait(&status) != pid)   // 等待子进程结束
        err_sys("wait error");  // 如果出错,打印错误信息
    pr_exit(status);            // 打印子进程退出状态

    if ((pid = fork()) < 0)     // 再次创建子进程
        err_sys("fork error");  // 如果出错,打印错误信息
    else if (pid == 0)          /* child */
        status /= 0;            // 子进程执行除以0操作,生成SIGFPE信号

    if (wait(&status) != pid)   // 等待子进程结束
        err_sys("wait error");  // 如果出错,打印错误信息
    pr_exit(status);            // 打印子进程退出状态

    exit(0);  // 主进程退出,返回状态码0
}
