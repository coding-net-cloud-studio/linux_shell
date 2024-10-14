#include <signal.h>  // 包含信号处理函数的头文件
#include <unistd.h>  // 包含系统调用和基本常量的头文件

// 信号处理函数,当接收到SIGALRM信号时调用
// 参数signo表示接收到的信号编号
static void
sig_alrm(int signo)
{
    /* 什么都不做,只是返回以唤醒pause */
}

/**
 * sleep1 - 暂停指定的秒数
 * @seconds: 需要暂停的秒数
 *
 * 返回值: 实际未睡眠的时间(如果被信号中断)
 */
unsigned int
sleep1(unsigned int seconds)
{
    // 设置SIGALRM信号的处理函数为sig_alrm
    // 如果设置失败,返回传入的秒数
    if (signal(SIGALRM, sig_alrm) == SIG_ERR)
        return (seconds);

    // 启动定时器,经过seconds秒后发送SIGALRM信号
    alarm(seconds);

    // 暂停进程,直到接收到信号
    pause();

    // 关闭定时器,并返回未睡眠的时间
    return (alarm(0));
}
