#include "apue.h"  // 引入 apue.h 头文件,该文件包含了一些系统编程所需的常量和函数声明

static void sig_quit(int);  // 声明信号处理函数 sig_quit

/**
 * 主函数,程序的入口点.
 */
int main(void)
{
    sigset_t newmask, oldmask, pendmask;  // 定义信号集变量,用于存储新的,旧的和未决的信号集

    // 注册信号处理函数,捕获 SIGQUIT 信号
    if (signal(SIGQUIT, sig_quit) == SIG_ERR)
        // 如果注册失败,调用 err_sys 函数输出错误信息
        err_sys("can't catch SIGQUIT");

    /*
     * 阻塞 SIGQUIT 信号,并保存当前的信号掩码.
     */
    sigemptyset(&newmask);         // 初始化新的信号集为空
    sigaddset(&newmask, SIGQUIT);  // 将 SIGQUIT 信号添加到新的信号集中

    // 使用 sigprocmask 函数阻塞 SIGQUIT 信号,并保存旧的信号掩码
    if (sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0)
        // 如果设置信号掩码失败,调用 err_sys 函数输出错误信息
        err_sys("SIG_BLOCK error");

    sleep(5);  // 程序休眠 5 秒,期间如果有 SIGQUIT 信号发送,将会被阻塞

    // 检查是否有未决的信号
    if (sigpending(&pendmask) < 0)
        // 如果获取未决信号集失败,调用 err_sys 函数输出错误信息
        err_sys("sigpending error");

    // 检查 SIGQUIT 是否在未决的信号集中
    if (sigismember(&pendmask, SIGQUIT))
        // 如果是,打印一条消息表示 SIGQUIT 信号是未决的
        printf("\nSIGQUIT pending\n");

    /*
     * 恢复之前的信号掩码,即解除对 SIGQUIT 信号的阻塞.
     */
    // 使用 sigprocmask 函数恢复旧的信号掩码
    if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0)
        // 如果设置信号掩码失败,调用 err_sys 函数输出错误信息
        err_sys("SIG_SETMASK error");
    printf("SIGQUIT unblocked\n");  // 打印一条消息表示 SIGQUIT 信号已解除阻塞

    sleep(5);  // 程序再次休眠 5 秒,期间如果有 SIGQUIT 信号发送,将会被处理

    exit(0);  // 程序正常结束,退出状态码为 0
}

/**
 * 捕获 SIGQUIT 信号时调用的函数.
 * 打印一条消息表示捕获到 SIGQUIT 信号,并尝试将其恢复为默认行为.
 * @param signo 信号编号
 */
static void
sig_quit(int signo)
{
    printf("caught SIGQUIT\n");  // 打印捕获到的信号信息
    // 尝试将 SIGQUIT 信号的处理方式恢复为默认方式
    if (signal(SIGQUIT, SIG_DFL) == SIG_ERR)
        // 如果恢复失败,调用 err_sys 函数输出错误信息
        err_sys("can't reset SIGQUIT");
}
