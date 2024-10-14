#include "apue.h"
#include <sys/wait.h>

// 信号处理函数,用于处理子进程结束的信号
static void
sig_cld(int signo);

int main()
{
    pid_t pid;

    // 设置 SIGCLD 信号的处理函数为 sig_cld
    if (signal(SIGCLD, sig_cld) == SIG_ERR)
        perror("signal error");  // 如果设置信号处理函数失败,打印错误信息

    // 创建子进程
    if ((pid = fork()) < 0)
    {
        perror("fork error");  // 如果 fork 失败,打印错误信息
    }
    else if (pid == 0)
    {              /* child */
        sleep(2);  // 子进程休眠 2 秒
        _exit(0);  // 子进程结束
    }

    pause(); /* parent */  // 父进程暂停,等待信号
    exit(0);
}

static void
sig_cld(int signo) /* 中断 pause() */
{
    pid_t pid;     // 定义进程ID变量
    int   status;  // 定义状态变量

    printf("SIGCLD received\n");  // 打印接收到的信号

    if (signal(SIGCLD, sig_cld) == SIG_ERR) /* 重新建立处理函数 */
        perror("signal error");             // 如果信号处理函数设置失败,打印错误信息

    if ((pid = wait(&status)) < 0) /* 获取子进程状态 */
        perror("wait error");      // 如果等待子进程失败,打印错误信息

    printf("pid = %d\n", pid);  // 打印结束的子进程ID
}
