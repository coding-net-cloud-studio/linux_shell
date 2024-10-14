#include "apue.h"     // 引入APUE库的头文件
#include <pthread.h>  // 引入POSIX线程库的头文件

// 初始化互斥锁lock1,用于保护共享资源,防止多个线程同时访问造成数据不一致
pthread_mutex_t lock1 = PTHREAD_MUTEX_INITIALIZER;

// 初始化互斥锁lock2,同样用于保护共享资源
pthread_mutex_t lock2 = PTHREAD_MUTEX_INITIALIZER;

// 准备锁的函数,用于在启动线程之前锁定两个互斥锁
void prepare(void)
{
    int err;  // 用于存储错误代码

    printf("preparing locks...\n");  // 打印准备锁的信息
    // 尝试锁定第一个互斥锁,如果失败则调用err_cont函数处理错误
    if ((err = pthread_mutex_lock(&lock1)) != 0)
        err_cont(err, "can't lock lock1 in prepare handler");
    // 尝试锁定第二个互斥锁,如果失败则调用err_cont函数处理错误
    if ((err = pthread_mutex_lock(&lock2)) != 0)
        err_cont(err, "can't lock lock2 in prepare handler");
}

void parent(void)
{
    int err;

    // 打印父线程解锁锁的信息
    printf("parent unlocking locks...\n");

    // 尝试解锁lock1,如果失败则调用err_cont函数处理错误
    // 如果解锁互斥锁lock1失败,将错误信息传递给err_cont函数
    // pthread_mutex_unlock: 解锁互斥锁的函数
    // &lock1: 指向需要解锁的互斥锁的指针
    // err_cont: 错误处理函数,用于处理错误信息
    if ((err = pthread_mutex_unlock(&lock1)) != 0)              // 尝试解锁lock1
        err_cont(err, "can't unlock lock1 in parent handler");  // 解锁失败,调用err_cont函数处理错误

    // 尝试解锁lock2,如果失败则调用err_cont函数处理错误
    if ((err = pthread_mutex_unlock(&lock2)) != 0)
        err_cont(err, "can't unlock lock2 in parent handler");
}

void child(void)
{
    int err;

    // 打印子线程解锁锁的信息
    printf("child unlocking locks...\n");

    // 尝试解锁lock1,如果失败则调用err_cont函数报告错误
    if ((err = pthread_mutex_unlock(&lock1)) != 0)
        err_cont(err, "can't unlock lock1 in child handler");

    // 尝试解锁lock2,如果失败则调用err_cont函数报告错误
    // 如果解锁互斥锁lock2失败,则调用err_cont函数报告错误
    if ((err = pthread_mutex_unlock(&lock2)) != 0)
        // err_cont函数的第一个参数是错误代码,第二个参数是错误信息
        err_cont(err, "can't unlock lock2 in child handler");
}

void *
thr_fn(void *arg)  // 定义线程函数thr_fn,参数为void指针类型
{
    printf("thread started...\n");  // 打印线程开始执行的消息
    pause();                        // 暂停线程执行,等待信号
    return (0);                     // 线程执行完毕后返回0
}

// fig12.17.c

/**
 * 主函数,演示了pthread_atfork的使用,该函数用于注册在fork之前,之后(父进程)和之后(子进程)执行的函数.
 */
int main(void)
{
    int       err;  // 错误码
    pid_t     pid;  // 进程ID
    pthread_t tid;  // 线程ID

    /**
     * 注册在fork之前,父进程之后和子进程之后执行的函数.
     * @param prepare 在fork之前执行的函数
     * @param parent  在父进程fork之后执行的函数
     * @param child   在子进程fork之后执行的函数
     * @return         如果注册成功返回0,否则返回错误码
     */
    if ((err = pthread_atfork(prepare, parent, child)) != 0)
        err_exit(err, "can't install fork handlers");

    /**
     * 创建一个新线程.
     * @param &tid     指向新创建线程ID的指针
     * @param NULL     线程属性,这里使用默认属性
     * @param thr_fn   新线程开始执行的函数
     * @param 0        传递给新线程函数的参数
     * @return         如果线程创建成功返回0,否则返回错误码
     */
    if ((err = pthread_create(&tid, NULL, thr_fn, 0)) != 0)
        err_exit(err, "can't create thread");

    sleep(2);  // 主线程休眠2秒
    printf("parent about to fork...\n");

    /**
     * 创建子进程.
     * @return         如果fork失败返回-1,如果创建的是子进程返回0,如果是父进程返回子进程的PID
     */
    // 使用fork系统调用创建一个新进程
    if ((pid = fork()) < 0)
    {
        // 如果pid小于0,表示fork失败,输出错误信息并退出程序
        err_quit("fork failed");
    }
    else if (pid == 0)
    { /* child */
        // 如果pid等于0,表示当前代码在子进程中执行
        printf("child returned from fork\n");
    }
    else
    { /* parent */
        // 如果pid大于0,表示当前代码在父进程中执行
        printf("parent returned from fork\n");
    }

    exit(0);  // 退出进程
}
