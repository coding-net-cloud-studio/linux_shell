#include <signal.h>
#include <unistd.h>

// 信号处理函数,用于处理定时信号
// 参数 signo 是接收到的信号编号
static void
sig_alrm(int signo)
{
    /* 什么都不做,只是返回以唤醒 pause */
    // 当定时器超时时,会发送 SIGALRM 信号,此函数会被调用
    // 函数内不需要执行任何操作,因为它的作用仅仅是作为 pause 函数的唤醒点
}

unsigned int
sleep1(unsigned int seconds)
{
    // 如果设置SIGALRM信号的处理器失败,返回传入的秒数
    if (signal(SIGALRM, sig_alrm) == SIG_ERR)
        return (seconds);
    // 启动定时器,经过指定的秒数后发送SIGALRM信号
    alarm(seconds);
    // 暂停进程执行,直到捕获到信号为止
    pause();
    // 关闭定时器并返回未睡眠的时间
    return (alarm(0));
}
