#include "apue.h"  // 引入apue库,该库提供了一系列系统编程的工具和函数

// @brief 设置信号处理函数
// @param signo 需要处理的信号编号
// @param func 信号处理函数指针
// @return 返回之前的信号处理函数指针,如果出错则返回SIG_ERR
Sigfunc *
signal_intr(int signo, Sigfunc *func)
{
    struct sigaction act, oact;  // 定义两个sigaction结构体,act用于设置新的信号处理动作,oact用于保存原有的信号处理动作

    act.sa_handler = func;      // 设置新的信号处理函数
    sigemptyset(&act.sa_mask);  // 清空信号掩码集,表示处理该信号时不阻塞其他信号
    act.sa_flags = 0;           // 初始化sa_flags标志位

#ifdef SA_INTERRUPT
    act.sa_flags |= SA_INTERRUPT;  // 如果定义了SA_INTERRUPT,则设置SA_INTERRUPT标志位,表示信号处理函数执行完毕后不会恢复被中断的系统调用
#endif

    if (sigaction(signo, &act, &oact) < 0)  // 调用sigaction函数设置信号处理动作
        return (SIG_ERR);                   // 如果出错,返回SIG_ERR
    return (oact.sa_handler);               // 返回之前的信号处理函数指针
}
