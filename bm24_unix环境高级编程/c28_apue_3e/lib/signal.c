#include "apue.h"

/*
 * signal函数的可靠版本,使用POSIX sigaction().
 *
 * @param signo 信号编号
 * @param func 信号处理函数指针
 * @return 如果成功,返回之前的信号处理函数指针;如果失败,返回SIG_ERR
 */
Sigfunc *
signal(int signo, Sigfunc *func)
{
    struct sigaction act, oact;  // 定义两个sigaction结构体,act用于设置新的信号处理动作,oact用于保存原有的信号处理动作

    act.sa_handler = func;      // 设置新的信号处理函数
    sigemptyset(&act.sa_mask);  // 清空信号掩码集,表示在信号处理函数执行期间不阻塞其他信号
    act.sa_flags = 0;           // 初始化sa_flags标志位

    // 根据信号类型设置sa_flags标志位
    if (signo == SIGALRM)
    {
#ifdef SA_INTERRUPT
        act.sa_flags |= SA_INTERRUPT;  // 如果定义了SA_INTERRUPT,则设置为中断模式
#endif
    }
    else
    {
        act.sa_flags |= SA_RESTART;  // 其他信号设置为重启模式
    }

    // 调用sigaction函数设置信号处理动作
    if (sigaction(signo, &act, &oact) < 0)
        return (SIG_ERR);  // 如果失败,返回SIG_ERR

    return (oact.sa_handler);  // 如果成功,返回之前的信号处理函数指针
}
