#include "apue.h"

static volatile sig_atomic_t sigflag; /* set nonzero by sig handler */
static sigset_t              newmask, oldmask, zeromask;

static void
sig_usr(int signo) /* 一个信号处理函数,用于处理SIGUSR1和SIGUSR2信号 */
{
    sigflag = 1;  // 当接收到信号时,设置全局变量sigflag为1
}

// TELL_WAIT函数用于设置信号处理程序并阻塞SIGUSR1和SIGUSR2信号.
// 这样做是为了在进程间同步,其中一个进程可以使用SIGUSR1通知另一个进程开始执行某个操作,
// 然后使用SIGUSR2等待另一个进程完成该操作.
void TELL_WAIT(void)
{
    // 设置SIGUSR1信号的处理程序为sig_usr函数
    if (signal(SIGUSR1, sig_usr) == SIG_ERR)
        err_sys("signal(SIGUSR1) error");

    // 设置SIGUSR2信号的处理程序为sig_usr函数
    if (signal(SIGUSR2, sig_usr) == SIG_ERR)
        err_sys("signal(SIGUSR2) error");

    // 清空zeromask信号集
    sigemptyset(&zeromask);
    // 清空newmask信号集
    sigemptyset(&newmask);
    // 将SIGUSR1信号添加到newmask信号集中
    sigaddset(&newmask, SIGUSR1);
    // 将SIGUSR2信号添加到newmask信号集中
    sigaddset(&newmask, SIGUSR2);

    // 阻塞SIGUSR1和SIGUSR2信号,并保存当前的信号掩码
    if (sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0)
        err_sys("SIG_BLOCK error");
}

/**
 * TELL_PARENT - 向父进程发送信号,表明子进程已完成
 * @pid: 父进程的进程ID
 *
 * 该函数通过发送SIGUSR2信号通知父进程,子进程已经完成了它的任务.
 * 父进程可以通过捕获这个信号来执行相应的操作.
 */
void TELL_PARENT(pid_t pid)
{
    kill(pid, SIGUSR2); /* 告诉父进程我们已完成 */
}

/**
 * @brief 等待父进程的信号
 *
 * 该函数通过检查全局变量sigflag的值来等待父进程的信号.如果sigflag为0,则调用sigsuspend函数
 * 暂停当前进程的执行,并将信号屏蔽设置为zeromask.当父进程发送信号后,sigflag会被设置为非0值,
 * 此时WAIT_PARENT函数会退出循环,并重置信号屏蔽为原来的值.
 */
void WAIT_PARENT(void)
{
    // 循环等待父进程设置sigflag为非0值
    while (sigflag == 0)
        sigsuspend(&zeromask); /* and wait for parent */  // 暂停进程,等待信号
    sigflag = 0;                                          // 重置sigflag,以便下次等待

    /* 重置信号屏蔽为原始值 */
    if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0)
        err_sys("SIG_SETMASK error");  // 如果重置信号屏蔽失败,则输出错误信息
}

/**
 * @brief 通知子进程我们已经完成某个操作
 *
 * 该函数通过发送SIGUSR1信号给指定的子进程,告知它父进程已经完成了某个操作.
 * 子进程可以通过信号处理函数来响应这个信号,并执行相应的操作.
 *
 * @param pid 子进程的进程ID
 */
void TELL_CHILD(pid_t pid)
{
    kill(pid, SIGUSR1); /* 告诉子进程我们完成了 */
}

/**
 * @brief 等待子进程结束的函数
 *
 * 该函数通过检查全局变量sigflag的值来判断子进程是否已经结束.
 * 如果sigflag为0,则调用sigsuspend函数挂起当前进程的执行,
 * 直到接收到一个信号.这里使用的是zeromask,意味着挂起时不会阻塞任何信号.
 * 当子进程结束时,会发送一个信号,导致sigflag被设置为非0值,
 * 然后父进程会从sigsuspend返回并继续执行.
 *
 * @return void
 */
void WAIT_CHILD(void)
{
    // 循环检查sigflag,直到子进程结束信号被接收
    while (sigflag == 0)
        sigsuspend(&zeromask); /* 等待子进程 */

    // 重置sigflag,以便下次等待
    sigflag = 0;

    /* 将信号掩码重置为原始值 */
    if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0)
        err_sys("SIG_SETMASK error");  // 如果出错,打印错误信息
}
