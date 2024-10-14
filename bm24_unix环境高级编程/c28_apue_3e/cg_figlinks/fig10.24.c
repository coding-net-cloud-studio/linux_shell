#include "apue.h"  // 引入apue库,该库提供了一系列系统编程的工具和函数

// 定义一个全局变量,用于信号处理函数中设置标志位
static volatile sig_atomic_t sigflag;

// 定义信号集,用于保存和操作信号掩码
static sigset_t newmask, oldmask, zeromask;

/**
 * @brief 信号处理函数,用于处理SIGUSR1和SIGUSR2信号
 * @param signo 接收到的信号编号
 */
static void sig_usr(int signo)
{
    sigflag = 1;  // 设置标志位,表示信号已被接收
}

/**
 * @brief 初始化信号处理,阻塞SIGUSR1和SIGUSR2信号
 */
void TELL_WAIT(void)
{
    // 设置SIGUSR1和SIGUSR2的信号处理函数为sig_usr
    // 为SIGUSR1信号注册处理函数sig_usr,如果注册失败,则调用err_sys输出错误信息
    if (signal(SIGUSR1, sig_usr) == SIG_ERR)
        err_sys("signal(SIGUSR1) error");

    // 为SIGUSR2信号注册处理函数sig_usr,如果注册失败,则调用err_sys输出错误信息
    if (signal(SIGUSR2, sig_usr) == SIG_ERR)
        err_sys("signal(SIGUSR2) error");

    // 初始化信号集
    // 清空信号集zeromask
    sigemptyset(&zeromask);
    // 清空信号集newmask
    sigemptyset(&newmask);
    // 向newmask信号集添加SIGUSR1信号
    sigaddset(&newmask, SIGUSR1);
    // 向newmask信号集添加SIGUSR2信号
    sigaddset(&newmask, SIGUSR2);

    // 阻塞SIGUSR1和SIGUSR2信号,并保存当前的信号掩码
    // 使用sigprocmask函数阻塞信号集
    // SIG_BLOCK: 表示将新的信号集添加到进程的信号屏蔽字中,即阻塞这些信号
    // &newmask: 指向新的信号集的指针,这些信号将被阻塞
    // &oldmask: 如果不为空,则在函数返回前将旧的信号屏蔽字复制到这个位置
    // 返回值: 如果成功,返回0;如果出错,返回-1
    if (sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0)
        // 如果sigprocmask函数调用出错,打印错误信息并退出程序
        err_sys("SIG_BLOCK error");
}

/**
 * @brief 向父进程发送信号,告知已完成
 * @param pid 父进程的PID
 */
void TELL_PARENT(pid_t pid)
{
    kill(pid, SIGUSR2);  // 发送SIGUSR2信号给父进程
}

/**
 * @brief 等待父进程的信号
 */
void WAIT_PARENT(void)
{
    // 循环检查sigflag,直到被设置为非零值
    while (sigflag == 0)
        sigsuspend(&zeromask);  // 挂起进程,直到接收到信号

    sigflag = 0;  // 重置标志位

    // 恢复原来的信号掩码
    // 如果sigprocmask函数调用失败,返回值小于0,则通过err_sys函数报告"SIG_SETMASK error"错误.
    // sigprocmask函数用于改变进程的信号掩码.
    // SIG_SETMASK参数表示要设置新的信号掩码.
    // &oldmask参数是一个指向旧的信号掩码的指针,用于保存调用前的信号掩码.
    // NULL参数表示不需要保存未改变的信号掩码.
    if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0)
        err_sys("SIG_SETMASK error");
}

/**
 * @brief 向子进程发送信号,告知已完成
 * @param pid 子进程的PID
 */
void TELL_CHILD(pid_t pid)
{
    kill(pid, SIGUSR1);  // 发送SIGUSR1信号给子进程
}

/**
 * @brief 等待子进程的信号
 */
void WAIT_CHILD(void)
{
    // 循环检查sigflag,直到被设置为非零值
    while (sigflag == 0)
        sigsuspend(&zeromask);  // 挂起进程,直到接收到信号

    sigflag = 0;  // 重置标志位

    // 恢复原来的信号掩码
    // 如果sigprocmask函数调用失败,返回值小于0,则通过err_sys函数报告"SIG_SETMASK error"错误.
    // sigprocmask函数用于改变进程的信号掩码.
    // SIG_SETMASK参数表示要设置新的信号掩码.
    // &oldmask参数是一个指向旧的信号掩码的指针,用于保存调用前的信号掩码.
    // NULL参数表示不需要保存未改变的信号掩码.
    if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0)
        err_sys("SIG_SETMASK error");
}
