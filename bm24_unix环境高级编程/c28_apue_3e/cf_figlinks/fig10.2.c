#include "apue.h"

// 定义信号处理函数,用于处理SIGUSR1和SIGUSR2信号
static void sig_usr(int signo);

/**
 * 主函数
 * @return int 程序退出状态码
 */
int main(void)
{
    // 注册SIGUSR1信号的处理函数为sig_usr
    if (signal(SIGUSR1, sig_usr) == SIG_ERR)
    {
        err_sys("无法捕获SIGUSR1信号");
    }
    // 注册SIGUSR2信号的处理函数为sig_usr
    if (signal(SIGUSR2, sig_usr) == SIG_ERR)
    {
        err_sys("无法捕获SIGUSR2信号");
    }
    // 进入无限循环,等待信号
    for (;;)
    {
        pause();
    }
}

static void
sig_usr(int signo) /* argument is signal number */
{
    // 当接收到SIGUSR1信号时,打印消息
    if (signo == SIGUSR1)
        printf("received SIGUSR1\n");
    // 当接收到SIGUSR2信号时,打印消息
    else if (signo == SIGUSR2)
        printf("received SIGUSR2\n");
    // 如果接收到其他信号,调用err_dump函数打印错误消息
    else
        err_dump("received signal %d\n", signo);
}
