#include "apue.h"     // 引入apue库的头文件
#include <pthread.h>  // 引入POSIX线程库的头文件

int      quitflag;  // 由线程设置为非零值的标志,用于通知主线程退出
sigset_t mask;      // 信号集,用于存储被阻塞的信号

// 初始化互斥锁,用于保护共享资源
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
// 初始化条件变量,用于线程间的同步
pthread_cond_t waitloc = PTHREAD_COND_INITIALIZER;

// thr_fn 是一个线程函数,用于等待信号并在接收到特定信号时执行相应操作.
// 参数 arg 未在此函数中使用.
void *
thr_fn(void *arg)
{
    int err, signo;  // err 用于存储函数返回的错误代码,signo 用于存储接收到的信号编号.

    // 无限循环等待信号
    for (;;)
    {
        // 等待信号,mask 是一个信号集,包含线程愿意等待的信号.
        err = sigwait(&mask, &signo);
        if (err != 0)  // 如果 sigwait 返回错误,则调用 err_exit 函数处理错误并退出.
            err_exit(err, "sigwait failed");

        // 根据接收到的信号执行不同的操作
        switch (signo)
        {
        case SIGINT:                  // 如果接收到中断信号(通常是 Ctrl+C)
            printf("\ninterrupt\n");  // 打印中断信息
            break;                    // 结束 switch 语句

        case SIGQUIT:                       // 如果接收到退出信号
            pthread_mutex_lock(&lock);      // 加锁保护共享资源
            quitflag = 1;                   // 设置退出标志
            pthread_mutex_unlock(&lock);    // 解锁
            pthread_cond_signal(&waitloc);  // 唤醒等待条件变量的线程
            return (0);                     // 线程退出

        default:                                      // 如果接收到未处理的信号
            printf("unexpected signal %d\n", signo);  // 打印意外信号信息
            exit(1);                                  // 退出程序
        }
    }
}

int main(void)
{
    int       err;      // 错误码
    sigset_t  oldmask;  // 旧的信号掩码
    pthread_t tid;      // 线程ID

    sigemptyset(&mask);         // 清空信号集
    sigaddset(&mask, SIGINT);   // 向信号集添加SIGINT信号
    sigaddset(&mask, SIGQUIT);  // 向信号集添加SIGQUIT信号
    // 阻塞SIGINT和SIGQUIT信号,并保存旧的信号掩码
    // 如果信号屏蔽字设置失败,则调用err_exit函数处理错误
    // pthread_sigmask函数用于设置线程的信号屏蔽字
    // SIG_BLOCK表示要阻塞的信号集合
    // &mask 是指向信号集的指针,包含需要阻塞的信号
    // &oldmask 是一个输出参数,用于保存调用pthread_sigmask之前的信号屏蔽字
    // 如果函数返回值不为0,表示调用失败,err变量会包含错误码
    if ((err = pthread_sigmask(SIG_BLOCK, &mask, &oldmask)) != 0)
        err_exit(err, "SIG_BLOCK error");  // 调用err_exit函数,传入错误码和错误信息

    // 创建一个新线程
    // 创建一个新的线程
    // 参数:
    //   &tid: 指向新线程ID的指针
    //   NULL: 使用默认线程属性
    //   thr_fn: 线程函数的指针
    //   0: 传递给线程函数的参数(无)
    err = pthread_create(&tid, NULL, thr_fn, 0);

    // 检查线程是否成功创建
    // 如果err不为0,表示创建线程失败
    if (err != 0)
        // 调用err_exit函数处理错误,并输出错误信息"can't create thread"
        err_exit(err, "can't create thread");

    pthread_mutex_lock(&lock);  // 加锁
    // 等待quitflag变为1
    // 等待条件变量,直到被唤醒或发生中断
    // pthread_cond_wait 会自动释放锁,并在被唤醒后重新获取锁
    // 这使得其他线程有机会获取锁并执行相应的操作
    while (quitflag == 0)
    {
        // 等待条件变量 waitloc,同时释放锁 lock
        // 当条件满足或被信号唤醒时,将重新获取锁并继续执行
        pthread_cond_wait(&waitloc, &lock);
    }

    pthread_mutex_unlock(&lock);  // 解锁

    /* SIGQUIT已被捕获并现在被阻塞;执行相应操作 */
    quitflag = 0;

    /* 重置信号掩码,解除SIGQUIT的阻塞 */
    // 如果sigprocmask函数调用失败,返回值小于0,则调用err_sys函数报告"SIG_SETMASK error"错误.
    // sigprocmask函数用于改变进程的信号屏蔽字.
    // SIG_SETMASK参数表示要设置新的信号屏蔽字.
    // &oldmask参数是一个指向旧的信号屏蔽字的指针,用于保存调用前的信号屏蔽字.
    // NULL参数表示不需要保存被阻塞的信号集.
    if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0)
        err_sys("SIG_SETMASK error");

    exit(0);  // 退出程序
}
