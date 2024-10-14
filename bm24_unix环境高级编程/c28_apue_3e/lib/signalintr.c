#include "apue.h"

// @brief 设置信号处理函数
// @param signo 需要处理的信号编号
// @param func 指向用户定义的信号处理函数的指针
// @return 如果成功,返回之前的信号处理函数指针;如果失败,返回SIG_ERR
Sigfunc *
signal_intr(int signo, Sigfunc *func)
{
    struct sigaction act, oact;  // 定义两个sigaction结构体,act用于设置新的信号处理动作,oact用于保存原有的信号处理动作

    act.sa_handler = func;      // 设置新的信号处理函数
    sigemptyset(&act.sa_mask);  // 清空信号掩码集,表示处理信号时不阻塞其他信号
    act.sa_flags = 0;           // 初始化sa_flags为0

#ifdef SA_INTERRUPT
    act.sa_flags |= SA_INTERRUPT;  // 如果定义了SA_INTERRUPT,将其加入到sa_flags中,表示信号处理函数执行完毕后,中断的系统调用不会被重启
#endif

    if (sigaction(signo, &act, &oact) < 0)  // 调用sigaction函数设置信号处理动作
        return (SIG_ERR);                   // 如果出错,返回SIG_ERR

    return (oact.sa_handler);  // 返回原有的信号处理函数指针
}
