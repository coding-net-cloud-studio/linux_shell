#include "apue.h"

// <summary>
// 信号处理函数,用于处理 SIGQUIT 信号
// </summary>
// <param name="signo">接收到的信号编号</param>
static void sig_quit(int signo);

int main(void)
{
    // 定义三个信号集变量,用于存储新的信号屏蔽字,旧的信号屏蔽字以及挂起的信号集
    sigset_t newmask, oldmask, pendmask;

    // 设置 SIGQUIT 信号的处理函数为 sig_quit

    // 注释:捕获SIGQUIT信号,并在接收到该信号时调用sig_quit函数进行处理
    // 如果无法捕获SIGQUIT信号,则通过err_sys函数输出错误信息
    if (signal(SIGQUIT, sig_quit) == SIG_ERR)
        err_sys("can't catch SIGQUIT");  // 如果捕获信号失败,打印错误信息

    /*
     * 阻塞 SIGQUIT 信号并保存当前的信号掩码
     */
    sigemptyset(&newmask);                               // 清空 newmask
    sigaddset(&newmask, SIGQUIT);                        // 将 SIGQUIT 信号添加到 newmask
    if (sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0)  // 阻塞 SIGQUIT 信号
        err_sys("SIG_BLOCK error");

    sleep(5);  // 睡眠 5 秒,期间如果收到 SIGQUIT 信号,信号将会保持挂起状态

    // 检查是否有挂起的信号
    if (sigpending(&pendmask) < 0)
        err_sys("sigpending error");  // 如果sigpending函数调用失败,打印错误信息并退出

    // 检查SIGQUIT信号是否在挂起的信号集中
    if (sigismember(&pendmask, SIGQUIT))  // 如果有挂起的 SIGQUIT 信号
        printf("\nSIGQUIT pending\n");    // 打印SIGQUIT信号挂起的信息

    /*
     * 恢复信号掩码,解除对 SIGQUIT 的阻塞
     */
    if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0)  // 恢复旧的信号掩码
        err_sys("SIG_SETMASK error");
    printf("SIGQUIT unblocked\n");

    sleep(5);  // 再次睡眠 5 秒,期间如果收到 SIGQUIT 信号,程序将会终止并生成核心转储文件
    exit(0);
}

static void
sig_quit(int signo)  // 定义一个信号处理函数,用于处理SIGQUIT信号
{
    printf("caught SIGQUIT\n");               // 打印信息,表示已经捕获到SIGQUIT信号
    if (signal(SIGQUIT, SIG_DFL) == SIG_ERR)  // 将SIGQUIT信号的处理方式重置为默认处理方式
        err_sys("can't reset SIGQUIT");       // 如果重置失败,打印错误信息
}
