#include "apue.h"

// 定义一个volatile sig_atomic_t类型的变量quitflag,用于信号处理函数中设置非零值
volatile sig_atomic_t quitflag; /* set nonzero by signal handler */

/**
 * 信号处理函数,用于处理SIGINT和SIGQUIT信号
 * @param signo 接收到的信号编号
 */
static void
sig_int(int signo) /* one signal handler for SIGINT and SIGQUIT */
{
    if (signo == SIGINT)
    {
        // 如果接收到的是SIGINT信号,打印提示信息
        printf("\ninterrupt\n");
    }
    else if (signo == SIGQUIT)
    {
        // 如果接收到的是SIGQUIT信号,设置quitflag为1,用于主循环中的退出条件
        quitflag = 1; /* set flag for main loop */
    }
}
int main(void)
{
    sigset_t newmask, oldmask, zeromask;

    // 设置SIGINT信号的处理函数为sig_int
    if (signal(SIGINT, sig_int) == SIG_ERR)
        err_sys("signal(SIGINT) error");
    // 设置SIGQUIT信号的处理函数为sig_int
    if (signal(SIGQUIT, sig_int) == SIG_ERR)
        err_sys("signal(SIGQUIT) error");

    // 清空zeromask信号集
    sigemptyset(&zeromask);
    // 清空newmask信号集
    sigemptyset(&newmask);
    // 向newmask信号集添加SIGQUIT信号
    sigaddset(&newmask, SIGQUIT);

    /*
     * 阻塞SIGQUIT信号并保存当前的信号掩码
     */
    if (sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0)
        err_sys("SIG_BLOCK error");

    // 当quitflag为0时,挂起进程,直到收到信号
    while (quitflag == 0)
        sigsuspend(&zeromask);

    /*
     * SIGQUIT信号已被捕获并现在被阻塞;执行相应操作
     */
    quitflag = 0;

    /*
     * 重置信号掩码,解除SIGQUIT信号的阻塞
     */
    if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0)
        err_sys("SIG_SETMASK error");

    // 退出程序
    exit(0);
}
