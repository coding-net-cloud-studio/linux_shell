#include "apue.h"

// 定义一个全局变量,用于标志是否接收到了退出信号
volatile sig_atomic_t quitflag; /* set nonzero by signal handler */

/**
 * @brief 信号处理函数,用于处理SIGINT和SIGQUIT信号
 *
 * @param signo 接收到的信号编号
 */
static void
sig_int(int signo) /* one signal handler for SIGINT and SIGQUIT */
{
    if (signo == SIGINT)
        printf("\ninterrupt\n");  // 如果接收到SIGINT信号,打印提示信息
    else if (signo == SIGQUIT)
        quitflag = 1; /* set flag for main loop */  // 如果接收到SIGQUIT信号,设置退出标志
}
int main(void)
{
    // 声明了三个信号集变量,用于存储新的信号掩码,旧的信号掩码和空的信号掩码.
    // sigset_t 是一个信号集合类型,可以用来表示一组信号.
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

    // 阻塞SIGQUIT信号并保存当前的信号掩码
    if (sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0)
        err_sys("SIG_BLOCK error");

    // 挂起进程,直到quitflag被设置为非零值
    while (quitflag == 0)
        sigsuspend(&zeromask);

    // SIGQUIT信号已被捕获并阻塞,现在可以执行相关操作
    quitflag = 0;

    // 重置信号掩码,解除SIGQUIT信号的阻塞
    if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0)
        err_sys("SIG_SETMASK error");

    // 退出程序
    exit(0);
}
