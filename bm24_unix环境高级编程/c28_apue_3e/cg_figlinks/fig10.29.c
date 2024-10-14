#include "apue.h"  // 引入apue库,该库提供了一系列系统编程的工具和函数

// 信号处理函数,用于处理定时信号
static void
sig_alrm(int signo)
{
    /* nothing to do, just returning wakes up sigsuspend() */
    // 当接收到信号时,这个函数会被调用,但这里不需要做任何事情
}

// @brief 使当前进程休眠指定的秒数
// @param seconds 需要休眠的秒数
// @return 返回未休眠的秒数
unsigned int
sleep(unsigned int seconds)
{
    struct sigaction newact, oldact;              // 定义新的和旧的信号处理动作
    sigset_t         newmask, oldmask, suspmask;  // 定义新的和旧的信号掩码,以及挂起时的信号掩码
    unsigned int     unslept;                     // 定义未休眠的秒数

    /* set our handler, save previous information */
    newact.sa_handler = sig_alrm;          // 设置新的信号处理函数
    sigemptyset(&newact.sa_mask);          // 清空新信号处理函数的信号掩码
    newact.sa_flags = 0;                   // 设置新信号处理函数的标志位
    sigaction(SIGALRM, &newact, &oldact);  // 设置SIGALRM信号的处理动作,并保存旧的处理动作

    /* block SIGALRM and save current signal mask */
    sigemptyset(&newmask);                       // 清空新的信号掩码
    sigaddset(&newmask, SIGALRM);                // 将SIGALRM信号添加到新的信号掩码中
    sigprocmask(SIG_BLOCK, &newmask, &oldmask);  // 阻塞SIGALRM信号,并保存当前的信号掩码

    alarm(seconds);      // 设置定时器
    suspmask = oldmask;  // 保存挂起时的信号掩码

    /* make sure SIGALRM isn't blocked */
    sigdelset(&suspmask, SIGALRM);  // 确保SIGALRM信号不被阻塞

    /* wait for any signal to be caught */
    sigsuspend(&suspmask);  // 挂起进程,等待任何信号的到来

    /* some signal has been caught, SIGALRM is now blocked */

    unslept = alarm(0);  // 获取未休眠的秒数

    /* reset previous action */
    sigaction(SIGALRM, &oldact, NULL);  // 恢复SIGALRM信号的旧处理动作

    /* reset signal mask, which unblocks SIGALRM */
    sigprocmask(SIG_SETMASK, &oldmask, NULL);  // 恢复旧的信号掩码,从而解除SIGALRM信号的阻塞
    return (unslept);                          // 返回未休眠的秒数
}
