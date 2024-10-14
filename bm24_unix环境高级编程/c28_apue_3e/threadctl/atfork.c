#include "apue.h"     // 引入APUE库的头文件,该库提供了一套用于UNIX和类UNIX系统的应用程序编程接口
#include <pthread.h>  // 引入POSIX线程库的头文件,用于创建和管理线程

// 初始化两个互斥锁
// 初始化两个互斥锁,用于线程同步
// lock1 是第一个互斥锁
pthread_mutex_t lock1 = PTHREAD_MUTEX_INITIALIZER;

// lock2 是第二个互斥锁
pthread_mutex_t lock2 = PTHREAD_MUTEX_INITIALIZER;

// 准备处理函数,在fork之前调用
void prepare(void)
{
    int err;

    // 打印准备锁定信息
    printf("preparing locks...\n");
    // 尝试锁定lock1
    // 如果锁定失败,调用err_cont函数处理错误
    if ((err = pthread_mutex_lock(&lock1)) != 0)
        err_cont(err, "can't lock lock1 in prepare handler");
    // 尝试锁定lock2
    // 如果锁定失败,调用err_cont函数处理错误
    if ((err = pthread_mutex_lock(&lock2)) != 0)
        err_cont(err, "can't lock lock2 in prepare handler");
}

// 父进程处理函数,在fork之后调用
void parent(void)
{
    int err;

    // 打印父进程解锁锁的信息
    printf("parent unlocking locks...\n");
    // 解锁lock1
    // 如果解锁失败,调用err_cont函数处理错误
    if ((err = pthread_mutex_unlock(&lock1)) != 0)
        err_cont(err, "can't unlock lock1 in parent handler");
    // 解锁lock2
    // 如果解锁失败,调用err_cont函数处理错误
    if ((err = pthread_mutex_unlock(&lock2)) != 0)
        err_cont(err, "can't unlock lock2 in parent handler");
}

// 子进程处理函数,在fork之后调用
void child(void)
{
    int err;

    // 打印子进程解锁锁的信息
    printf("child unlocking locks...\n");
    // 解锁lock1,如果解锁失败则调用err_cont函数处理错误
    if ((err = pthread_mutex_unlock(&lock1)) != 0)
        err_cont(err, "can't unlock lock1 in child handler");
    // 解锁lock2,如果解锁失败则调用err_cont函数处理错误
    if ((err = pthread_mutex_unlock(&lock2)) != 0)
        err_cont(err, "can't unlock lock2 in child handler");
}

// 线程函数
void *
thr_fn(void *arg)
{
    /**
     * 打印线程开始的提示信息
     */
    printf("thread started...\n");

    /**
     * 暂停当前线程的执行,等待信号
     * @return void* 返回值始终为NULL
     */
    pause();

    /**
     * 线程执行完毕,返回NULL
     * @return void* 返回值始终为NULL
     */
    return (0);
}

int main(void)
{

    // 定义整型变量err,用于存储函数返回的错误码
    int err;

    // 定义pid_t类型的变量pid,用于存储进程ID
    pid_t pid;

    // 定义pthread_t类型的变量tid,用于存储线程ID
    pthread_t tid;

    // 安装fork处理函数
    if ((err = pthread_atfork(prepare, parent, child)) != 0)
        err_exit(err, "can't install fork handlers");
    // 创建线程
    if ((err = pthread_create(&tid, NULL, thr_fn, 0)) != 0)
        err_exit(err, "can't create thread");

    sleep(2);  // 主线程休眠2秒
    printf("parent about to fork...\n");

    // 创建子进程
    if ((pid = fork()) < 0)
        err_quit("fork failed");
    else if (pid == 0) /* 子进程 */
        printf("child returned from fork\n");
    else /* 父进程 */
        printf("parent returned from fork\n");
    exit(0);
}
