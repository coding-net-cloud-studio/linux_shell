#include "apue.h"

// <summary>
// 处理中断信号(SIGINT)的函数.
// 当程序收到中断信号时,会调用此函数并打印消息.
// </summary>
// <param name="signo">接收到的信号编号.</param>
static void
sig_int(int signo)
{
    printf("caught SIGINT\n");
}

// <summary>
// 处理子进程结束信号(SIGCHLD)的函数.
// 当程序收到子进程结束信号时,会调用此函数并打印消息.
// </summary>
// <param name="signo">接收到的信号编号.</param>
static void
sig_chld(int signo)
{
    printf("caught SIGCHLD\n");
}
// systest2.c

/**
 * 主函数,设置信号处理程序并执行系统命令.
 */
int main(void)
{
    // 设置SIGINT信号的处理程序为sig_int函数
    if (signal(SIGINT, sig_int) == SIG_ERR)
        err_sys("signal(SIGINT) error");  // 如果设置失败,打印错误信息并退出

    // 设置SIGCHLD信号的处理程序为sig_chld函数
    if (signal(SIGCHLD, sig_chld) == SIG_ERR)
        err_sys("signal(SIGCHLD) error");  // 如果设置失败,打印错误信息并退出

    // 执行/bin/ed命令,如果执行失败,打印错误信息并退出
    if (system("/bin/ed") < 0)
        err_sys("system() error");

    exit(0);  // 程序正常退出
}
