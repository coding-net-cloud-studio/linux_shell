#include "apue.h"

// 信号处理函数,当接收到 SIGINT 信号时调用
static void sig_int(int signo);

int main(void)
{
    // 声明了三个信号集变量,用于存储新的信号掩码,旧的信号掩码和等待的信号掩码
    // sigset_t 是一个信号集类型,用于表示一组信号
    sigset_t newmask, oldmask, waitmask;

    // 打印程序开始时的信号掩码
    pr_mask("program start: ");

    // 设置 SIGINT 信号的处理函数为 sig_int
    if (signal(SIGINT, sig_int) == SIG_ERR)
        err_sys("signal(SIGINT) error");

    // 初始化 waitmask,将 SIGUSR1 添加到 waitmask 中
    // 清空信号集,为设置新的信号做准备
    sigemptyset(&waitmask);

    // 将SIGUSR1信号添加到信号集中
    // SIGUSR1是一个用户自定义信号,可以被应用程序用于任何目的
    sigaddset(&waitmask, SIGUSR1);

    // 初始化 newmask,将 SIGINT 添加到 newmask 中
    // 清空新的信号集
    sigemptyset(&newmask);

    // 将SIGINT信号添加到新的信号集中
    // SIGINT信号通常用于通知进程终止,例如用户按下Ctrl+C
    sigaddset(&newmask, SIGINT);

    // 阻塞 SIGINT 信号并保存当前的信号掩码
    if (sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0)
        err_sys("SIG_BLOCK error");

    // 临界区代码
    pr_mask("in critical region: ");

    // 暂停执行,等待除 SIGUSR1 之外的所有信号
    if (sigsuspend(&waitmask) != -1)
        err_sys("sigsuspend error");

    // 从 sigsuspend 返回后的信号掩码
    pr_mask("after return from sigsuspend: ");

    // 重置信号掩码,解除对 SIGINT 的阻塞
    if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0)
        err_sys("SIG_SETMASK error");

    // 继续处理其他事务
    pr_mask("program exit: ");

    exit(0);
}

// 当接收到 SIGINT 信号时的处理函数
static void sig_int(int signo)
{
    // 打印接收到 SIGINT 信号时的信号掩码
    pr_mask("\nin sig_int: ");
}
