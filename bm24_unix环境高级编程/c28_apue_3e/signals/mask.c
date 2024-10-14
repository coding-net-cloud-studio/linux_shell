#include "apue.h"    // 包含APUE库的头文件
#include <setjmp.h>  // 包含setjmp.h头文件,用于非局部跳转
#include <time.h>    // 包含time.h头文件,用于时间函数

// 定义信号处理函数sig_usr1,用于处理SIGUSR1信号
static void sig_usr1(int signo);

// 定义信号处理函数sig_alrm,用于处理SIGALRM信号
static void sig_alrm(int signo);

// 定义一个sigjmp_buf类型的变量jmpbuf,用于保存非局部跳转的环境
static sigjmp_buf jmpbuf;

// 定义一个volatile sig_atomic_t类型的变量canjump,用于标志是否可以进行非局部跳转
static volatile sig_atomic_t canjump;
// main.c

/**
 * 主函数,设置信号处理程序并进入主循环.
 */
int main(void)
{
    // 设置SIGUSR1信号的处理程序为sig_usr1,如果出错则调用err_sys打印错误信息
    if (signal(SIGUSR1, sig_usr1) == SIG_ERR)
        err_sys("signal(SIGUSR1) error");

    // 设置SIGALRM信号的处理程序为sig_alrm,如果出错则调用err_sys打印错误信息
    if (signal(SIGALRM, sig_alrm) == SIG_ERR)
        err_sys("signal(SIGALRM) error");

    // 打印当前信号掩码,表示main函数开始
    pr_mask("starting main: "); /* {Prog prmask} */

    // 使用sigsetjmp设置非局部跳转点,如果siglongjmp被调用,则跳转到此处
    if (sigsetjmp(jmpbuf, 1))
    {
        // 如果从siglongjmp跳转回来,打印当前信号掩码,表示main函数结束
        pr_mask("ending main: ");

        // 退出程序
        exit(0);
    }

    // 设置canjump标志,表示现在可以安全地使用sigsetjmp
    canjump = 1; /* now sigsetjmp() is OK */

    // 进入无限循环,等待信号
    for (;;)
        pause();
}

static void
sig_usr1(int signo)  // 定义信号处理函数sig_usr1,接收信号编号作为参数
{
    time_t starttime;  // 定义时间变量starttime

    if (canjump == 0)  // 如果canjump为0,表示不期望的信号,直接返回忽略
        return;

    pr_mask("starting sig_usr1: ");  // 打印开始处理sig_usr1信号的信息

    alarm(3);                            // 设置3秒后发送SIGALRM信号
    starttime = time(NULL);              // 记录当前时间
    for (;;)                             // 无限循环,忙等待5秒
        if (time(NULL) > starttime + 5)  // 如果当前时间超过开始时间5秒,则跳出循环
            break;

    pr_mask("finishing sig_usr1: ");  // 打印结束处理sig_usr1信号的信息

    canjump = 0;            // 将canjump设置为0,表示信号处理完成
    siglongjmp(jmpbuf, 1);  // 使用siglongjmp跳转回main函数,不返回
}
static void
sig_alrm(int signo)  // 定义信号处理函数sig_alrm,当接收到特定信号时会被调用
{
    pr_mask("in sig_alrm: ");  // 打印当前信号屏蔽字,用于调试信号处理时的状态
}
