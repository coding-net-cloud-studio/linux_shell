#include "apue.h"

// <summary>
// 信号处理函数,用于处理闹钟信号.
// 当接收到闹钟信号时,此函数会被调用.
// 目前函数体为空,仅通过返回来唤醒sigsuspend()函数.
// </summary>
// <param name="signo">接收到的信号编号</param>
static void
sig_alrm(int signo)
{
    /* nothing to do, just returning wakes up sigsuspend() */
}

unsigned int
sleep(unsigned int seconds)
{
    // 定义新的和旧的信号处理动作结构体
    struct sigaction newact, oldact;
    // 定义新的和旧的信号掩码集合,以及挂起时的信号掩码集合
    sigset_t newmask, oldmask, suspmask;
    // 定义未睡眠的时间
    unsigned int unslept;

    /* 设置我们的信号处理函数,并保存之前的信息 */
    newact.sa_handler = sig_alrm;          // 设置SIGALRM信号的处理函数为sig_alrm
    sigemptyset(&newact.sa_mask);          // 清空新动作的信号掩码
    newact.sa_flags = 0;                   // 设置新动作的标志位为0
    sigaction(SIGALRM, &newact, &oldact);  // 设置SIGALRM信号的动作,并保存旧的动作

    /* 阻塞SIGALRM信号并保存当前的信号掩码 */
    sigemptyset(&newmask);                       // 清空新的信号掩码集合
    sigaddset(&newmask, SIGALRM);                // 在新的信号掩码集合中添加SIGALRM信号
    sigprocmask(SIG_BLOCK, &newmask, &oldmask);  // 阻塞SIGALRM信号,并保存旧的信号掩码

    alarm(seconds);      // 设置闹钟,在seconds秒后发送SIGALRM信号
    suspmask = oldmask;  // 保存挂起时的信号掩码

    /* 确保SIGALRM信号不被阻塞 */
    sigdelset(&suspmask, SIGALRM);  // 从挂起时的信号掩码中移除SIGALRM信号

    /* 等待任何信号的捕获 */
    sigsuspend(&suspmask);  // 挂起进程,直到接收到任何信号

    /* 有信号被捕获,SIGALRM现在被阻塞 */

    unslept = alarm(0);  // 获取未睡眠的时间

    /* 重置之前的动作 */
    sigaction(SIGALRM, &oldact, NULL);  // 恢复SIGALRM信号的旧动作

    /* 重置信号掩码,这将解除SIGALRM的阻塞 */
    sigprocmask(SIG_SETMASK, &oldmask, NULL);  // 恢复旧的信号掩码
    return (unslept);                          // 返回未睡眠的时间
}
