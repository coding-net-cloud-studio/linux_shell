#include "apue.h"
#include <pthread.h>

// 全局变量,用于存储新线程的线程ID
pthread_t ntid;

// 打印进程ID和线程ID的函数
/*
 * @param s: 打印信息的前缀
 */
void printids(const char *s)
{
    pid_t     pid;  // 进程ID
    pthread_t tid;  // 线程ID

    pid = getpid();                                                                                         // 获取当前进程的ID
    tid = pthread_self();                                                                                   // 获取当前线程的ID
    printf("%s pid %lu tid %lu (0x%lx)\n", s, (unsigned long)pid, (unsigned long)tid, (unsigned long)tid);  // 打印进程ID和线程ID
}

// 新线程执行的函数
/*
 * @param arg: 线程函数的参数
 * @return: 返回值,这里返回NULL
 */
void *
thr_fn(void *arg)
{
    printids("new thread: ");  // 打印新线程的ID信息
    return ((void *)0);        // 返回NULL
}

/**
 * 主函数,创建一个新线程并打印线程ID.
 */
int main(void)
{
    int err;  // 用于存储pthread_create函数的返回值

    // 创建一个新线程,ntid将存储新线程的ID,thr_fn是新线程的入口函数
    err = pthread_create(&ntid, NULL, thr_fn, NULL);
    if (err != 0)
    {
        // 如果创建线程失败,调用err_exit函数处理错误
        err_exit(err, "can't create thread");
    }
    // 打印主线程的ID和提示信息
    printids("main thread:");
    sleep(1);  // 主线程休眠1秒,以等待新线程执行完毕
    exit(0);   // 程序正常退出
}
