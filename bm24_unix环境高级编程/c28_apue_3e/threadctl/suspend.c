#include "apue.h"     // 引入APUE库的头文件,该库提供了一套用于编写可移植UNIX应用程序的函数.
#include <pthread.h>  // 引入POSIX线程库的头文件,用于创建和管理线程.

// 定义退出标志,由线程设置为非零值
int quitflag; /* set nonzero by thread */
// 定义信号集
sigset_t mask;

// 初始化互斥锁
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
// 初始化条件变量
pthread_cond_t waitloc = PTHREAD_COND_INITIALIZER;

/**
 * 线程函数
 * @param arg 线程参数
 * @return 返回线程执行结果
 */
void *
thr_fn(void *arg)
{
    // 定义整型变量err和signo
    // err用于存储函数返回的错误代码
    // signo用于存储信号编号
    int err, signo;

    // 无限循环等待信号
    for (;;)
    {
        // 等待信号集中的信号
        err = sigwait(&mask, &signo);
        // 如果sigwait失败,打印错误并退出
        if (err != 0)
            err_exit(err, "sigwait failed");
        // 根据接收到的信号执行不同的操作
        switch (signo)
        {
        case SIGINT:
            // 接收到中断信号,打印提示信息
            printf("\ninterrupt\n");
            break;

        case SIGQUIT:
            // 接收到退出信号,锁定互斥锁
            pthread_mutex_lock(&lock);
            // 设置退出标志
            quitflag = 1;
            // 解锁互斥锁
            pthread_mutex_unlock(&lock);
            // 发送条件变量信号,通知等待的线程
            pthread_cond_signal(&waitloc);
            // 线程退出
            return (0);

        default:
            // 接收到未预期的信号,打印提示信息并退出线程
            printf("unexpected signal %d\n", signo);
            exit(1);
        }
    }
}

// 主函数,程序入口
int main(void)
{
    int       err;      // 错误码
    sigset_t  oldmask;  // 旧的信号掩码
    pthread_t tid;      // 线程标识符

    // 初始化信号集并添加SIGINT和SIGQUIT信号
    // 清空信号集
    sigemptyset(&mask);
    // 向信号集添加 SIGINT 信号,用于捕获 Ctrl+C 中断
    sigaddset(&mask, SIGINT);
    // 向信号集添加 SIGQUIT 信号,用于捕获 Ctrl+\ 退出
    sigaddset(&mask, SIGQUIT);

    // 阻塞SIGINT和SIGQUIT信号,并保存旧的信号掩码
    if ((err = pthread_sigmask(SIG_BLOCK, &mask, &oldmask)) != 0)
        err_exit(err, "SIG_BLOCK error");

    // 创建新线程
    // 创建一个新的线程
    // 参数:
    //   &tid: 线程标识符的指针,用于接收新创建线程的ID
    //   NULL: 使用默认线程属性
    //   thr_fn: 线程函数的指针,新线程将执行此函数
    //   0: 传递给线程函数的参数,此处为0
    err = pthread_create(&tid, NULL, thr_fn, 0);
    // 检查线程是否成功创建
    if (err != 0)
        // 如果创建失败,调用err_exit函数输出错误信息并退出程序
        err_exit(err, "can't create thread");

    // 锁定互斥量
    pthread_mutex_lock(&lock);
    // 等待条件变量,直到quitflag变为非零
    while (quitflag == 0)
        pthread_cond_wait(&waitloc, &lock);
    // 解锁互斥量
    pthread_mutex_unlock(&lock);

    /* SIGQUIT已被捕获并阻塞;执行相应操作 */
    quitflag = 0;

    // 重置信号掩码,解除SIGQUIT的阻塞
    // 如果sigprocmask函数调用失败,返回值小于0,则通过err_sys函数报告"SIG_SETMASK error"错误.
    // sigprocmask函数用于改变进程的信号屏蔽字.
    // SIG_SETMASK参数表示要设置的新的信号屏蔽字.
    // &oldmask是一个指向旧的信号屏蔽字的指针,用于保存调用前的信号屏蔽字状态.
    // NULL表示不需要保存被阻塞的信号集.
    if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0)
        err_sys("SIG_SETMASK error");

    // 程序正常退出
    exit(0);
}
