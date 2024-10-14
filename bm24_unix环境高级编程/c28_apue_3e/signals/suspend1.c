#include "apue.h"

// 信号处理函数,当接收到SIGINT信号时调用
static void
sig_int(int signo)
{
    pr_mask("\nin sig_int: ");  // 打印当前信号屏蔽字
}

int main(void)
{
    sigset_t newmask, oldmask, waitmask;  // 定义信号集

    pr_mask("program start: ");  // 程序开始时打印信号屏蔽字

    // 设置SIGINT信号的处理函数为sig_int
    // 为 SIGINT 信号(通常是用户按下 Ctrl+C)注册一个处理函数 sig_int
    // 如果 signal 函数返回 SIG_ERR,表示注册失败,调用 err_sys 函数报告错误
    if (signal(SIGINT, sig_int) == SIG_ERR)
        err_sys("signal(SIGINT) error");

    sigemptyset(&waitmask);         // 清空waitmask信号集
    sigaddset(&waitmask, SIGUSR1);  // 在waitmask信号集中添加SIGUSR1信号
    sigemptyset(&newmask);          // 清空newmask信号集
    sigaddset(&newmask, SIGINT);    // 在newmask信号集中添加SIGINT信号

    /*
     * 阻塞SIGINT信号并保存当前的信号屏蔽字
     */
    // 如果 sigprocmask 函数调用失败(返回值小于 0),则通过 err_sys 函数报告 "SIG_BLOCK error" 错误.
    // sigprocmask 用于改变进程的信号屏蔽字.
    // SIG_BLOCK 表示要阻塞 newmask 中包含的信号.
    // &newmask 是新信号屏蔽字的地址.
    // &oldmask 是一个指向 oldmask 变量的指针,在调用之前,oldmask 包含当前的信号屏蔽字.
    // 如果函数调用成功,oldmask 将被设置为调用 sigprocmask 之前的信号屏蔽字.
    if (sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0)
        err_sys("SIG_BLOCK error");

    pr_mask("in critical region: ");  // 进入临界区时打印信号屏蔽字

    /*
     * 暂停进程,此时除了SIGUSR1信号外,其他信号都会被阻塞
     */
    // sigsuspend函数用于临时替换进程的信号掩码,并暂停进程的执行,直到收到一个信号为止.
    // &waitmask 是一个信号集,表示进程当前愿意等待的信号.
    // 如果sigsuspend成功执行,它将返回-1,并设置errno以指示错误.
    // 如果进程被信号唤醒,sigsuspend将恢复原来的信号掩码并返回.
    // err_sys是一个自定义的错误处理函数,用于在发生系统错误时打印错误信息并退出程序.
    if (sigsuspend(&waitmask) != -1)
        err_sys("sigsuspend error");  // 如果sigsuspend返回值不是-1,表示发生了错误,调用err_sys函数处理错误

    pr_mask("after return from sigsuspend: ");  // 从sigsuspend返回后打印信号屏蔽字

    /*
     * 重置信号屏蔽字,解除对SIGINT的阻塞
     */
    // 检查sigprocmask函数调用的结果,如果返回值小于0,表示出现了错误
    if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0)
        // 调用err_sys函数输出错误信息"SIG_SETMASK error"
        err_sys("SIG_SETMASK error");

    pr_mask("program exit: ");  // 程序退出前打印信号屏蔽字

    exit(0);  // 程序正常退出
}
