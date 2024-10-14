#include <setjmp.h>  // 包含setjmp.h头文件,提供非局部跳转功能
#include <signal.h>  // 包含signal.h头文件,提供信号处理功能
#include <unistd.h>  // 包含unistd.h头文件,提供系统调用接口

// 定义一个静态的跳转缓冲区,用于longjmp函数
static jmp_buf env_alrm;

// 信号处理函数,当接收到SIGALRM信号时调用
static void
sig_alrm(int signo)
{
    // 使用longjmp跳回到setjmp的位置,并传递返回值1
    longjmp(env_alrm, 1);
}

/**
 * sleep2 - 模拟sleep函数,但允许被信号中断
 * @seconds: 需要休眠的秒数
 *
 * 返回值: 实际未休眠的时间
 */
unsigned int
sleep2(unsigned int seconds)
{
    // 设置SIGALRM信号的处理函数为sig_alrm
    if (signal(SIGALRM, sig_alrm) == SIG_ERR)
        return (seconds);  // 如果设置失败,直接返回传入的秒数

    // 使用setjmp设置跳转点,如果返回值为0,表示这是第一次执行到此处
    if (setjmp(env_alrm) == 0)
    {
        alarm(seconds);  // 启动定时器
        pause();         // 暂停进程,等待信号
    }
    // 关闭定时器,并返回未休眠的时间
    return (alarm(0));
}
