#include "apue.h"

static void sig_usr(int); /* one handler for both signals */

// <summary>
// 主函数,设置信号处理函数并进入无限循环等待信号
// </summary>
int main(void)
{
    // 设置SIGUSR1信号的处理函数为sig_usr
    if (signal(SIGUSR1, sig_usr) == SIG_ERR)
        err_sys("can't catch SIGUSR1");
    // 设置SIGUSR2信号的处理函数为sig_usr
    if (signal(SIGUSR2, sig_usr) == SIG_ERR)
        err_sys("can't catch SIGUSR2");
    // 进入无限循环,等待信号
    for (;;)
        pause();
}

static void
sig_usr(int signo) /* argument is signal number */
{
    /**
     * 处理用户定义的信号.
     * @param signo 信号编号
     */
    if (signo == SIGUSR1)
        printf("received SIGUSR1\n");  // 如果接收到SIGUSR1信号,打印消息
    else if (signo == SIGUSR2)
        printf("received SIGUSR2\n");  // 如果接收到SIGUSR2信号,打印消息
    else
        err_dump("received signal %d\n", signo);  // 如果接收到其他信号,打印错误信息
}
