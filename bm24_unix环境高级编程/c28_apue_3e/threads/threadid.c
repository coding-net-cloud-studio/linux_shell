#include "apue.h"     // 引入APUE库头文件
#include <pthread.h>  // 引入POSIX线程库头文件

pthread_t ntid;  // 定义一个全局的线程ID变量

/**
 * 打印进程ID和线程ID的函数
 * @param s 要打印的字符串前缀
 */
void printids(const char *s)
{
    pid_t     pid;  // 定义进程ID变量
    pthread_t tid;  // 定义线程ID变量

    pid = getpid();                                                                                         // 获取当前进程的ID
    tid = pthread_self();                                                                                   // 获取当前线程的ID
    printf("%s pid %lu tid %lu (0x%lx)\n", s, (unsigned long)pid, (unsigned long)tid, (unsigned long)tid);  // 打印进程ID和线程ID
}
/*
 * 线程函数,打印线程ID
 *
 * @param arg 传递给线程的参数
 * @return 返回NULL指针
 */
void *
thr_fn(void *arg)
{
    printids("new thread: ");  // 打印新线程的ID
    return ((void *)0);        // 返回NULL指针
}

/*
 * 主函数,创建一个新线程并打印主线程ID
 *
 * @return 返回0表示成功
 */
int main(void)
{
    int err;  // 错误码

    // 创建一个新线程,执行thr_fn函数,不传递参数
    err = pthread_create(&ntid, NULL, thr_fn, NULL);
    if (err != 0)                              // 如果创建线程失败
        err_exit(err, "can't create thread");  // 输出错误信息并退出程序
    printids("main thread:");                  // 打印主线程的ID
    sleep(1);                                  // 主线程休眠1秒,等待新线程执行完毕
    exit(0);                                   // 正常退出程序
}
