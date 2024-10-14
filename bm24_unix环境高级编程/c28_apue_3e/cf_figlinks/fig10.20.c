#include "apue.h"
#include <setjmp.h>
#include <time.h>

// 信号处理函数,处理 SIGUSR1 信号
// 参数:signo - 信号编号
static void sig_usr1(int signo);

// 信号处理函数,处理 SIGALRM 信号
// 参数:signo - 信号编号
static void sig_alrm(int signo);

// 用于 sigsetjmp 和 siglongjmp 的跳转缓冲区
static sigjmp_buf jmpbuf;

// 标志位,用于指示是否可以跳转
static volatile sig_atomic_t canjump;

int main(void)
{
    // 设置 SIGUSR1 信号的处理函数为 sig_usr1
    if (signal(SIGUSR1, sig_usr1) == SIG_ERR)
        err_sys("signal(SIGUSR1) error");
    // 设置 SIGALRM 信号的处理函数为 sig_alrm
    if (signal(SIGALRM, sig_alrm) == SIG_ERR)
        err_sys("signal(SIGALRM) error");

    pr_mask("starting main: "); /* {Prog prmask} */

    // 使用 sigsetjmp 设置跳转点,如果 canjump 被设置为非零值,则跳转到此处
    if (sigsetjmp(jmpbuf, 1))
    {
        pr_mask("ending main: ");
        exit(0);
    }
    canjump = 1; /* 现在可以安全地使用 sigsetjmp() */

    // 无限循环,等待信号
    for (;;)
        pause();
}

// 处理 SIGUSR1 信号的函数
static void sig_usr1(int signo)
{
    time_t starttime;

    // 如果 canjump 为 0,则忽略意外信号
    if (canjump == 0)
        return;

    pr_mask("starting sig_usr1: ");

    // 设置 3 秒后发送 SIGALRM 信号
    alarm(3);
    starttime = time(NULL);
    // 忙等待 5 秒
    for (;;)
        if (time(NULL) > starttime + 5)
            break;

    pr_mask("finishing sig_usr1: ");

    // 重置 canjump 标志位
    canjump = 0;
    // 使用 siglongjmp 跳转回 main 函数,不返回
    siglongjmp(jmpbuf, 1);
}

// 处理 SIGALRM 信号的函数
static void sig_alrm(int signo)
{
    pr_mask("in sig_almm: ");
}
