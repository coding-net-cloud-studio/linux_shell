#include "apue.h"

/*
 * 可靠版本的signal()函数,使用POSIX sigaction().
 * @param signo 信号编号
 * @param func 信号处理函数指针
 * @return 返回之前的信号处理函数指针,如果出错则返回SIG_ERR
 */
Sigfunc *
signal(int signo, Sigfunc *func)
{
    struct sigaction act, oact;  // 定义两个sigaction结构体,act用于设置新的信号处理动作,oact用于保存原有的信号处理动作

    act.sa_handler = func;      // 设置新的信号处理函数
    sigemptyset(&act.sa_mask);  // 清空信号掩码集,表示在信号处理函数执行期间不阻塞任何信号
    act.sa_flags = 0;           // 初始化sa_flags标志位

    // 根据信号类型设置SA_INTERRUPT或SA_RESTART标志位
    if (signo == SIGALRM)
    {
#ifdef SA_INTERRUPT
        act.sa_flags |= SA_INTERRUPT;  // 如果定义了SA_INTERRUPT,则设置该标志位,表示信号处理函数执行完毕后不自动重启被中断的系统调用
#endif
    }
    else
    {
        act.sa_flags |= SA_RESTART;  // 对于其他信号,设置SA_RESTART标志位,表示信号处理函数执行完毕后自动重启被中断的系统调用
    }

    // 调用sigaction()函数设置信号处理动作
    if (sigaction(signo, &act, &oact) < 0)
        return (SIG_ERR);  // 如果出错,返回SIG_ERR

    return (oact.sa_handler);  // 返回之前的信号处理函数指针
}
