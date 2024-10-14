#include "apue.h"
#include <pthread.h>

// 线程函数1
// 功能:打印线程1返回的信息,并返回一个指针值1
// 参数:arg - 传递给线程的参数
// 返回值:返回一个void指针,值为1
void *
thr_fn1(void *arg)
{
    // 线程1返回
    // 该函数用于打印线程1正在返回的消息,并返回一个指向void的指针,值为1.
    // 这通常用于表示线程的退出状态.
    printf("thread 1 returning\n");
    return ((void *)1);
}

// 线程函数2
// 功能:打印线程2退出的信息,并调用pthread_exit函数退出线程,返回一个指针值2
// 参数:arg - 传递给线程的参数
// 返回值:该函数不返回任何值,线程退出时通过pthread_exit返回一个void指针,值为2
void *
thr_fn2(void *arg)
{
    // 线程2退出
    // 该函数用于打印线程2退出的信息,并调用pthread_exit函数来终止线程,返回值为2.
    printf("thread 2 exiting\n");
    // pthread_exit函数用于终止调用它的线程,并可以传递一个指针作为线程的退出状态.
    // 这里的(void *)2表示线程退出时返回的状态码为2.
    pthread_exit((void *)2);
}
// exitstatus.c

/**
 * 主函数,创建两个线程并等待它们结束,打印线程的退出码.
 */
int main(void)
{
    int       err;         // 错误码
    pthread_t tid1, tid2;  // 线程标识符
    void     *tret;        // 线程返回值指针

    // 创建线程1
    // 创建线程1
    // pthread_create: 创建一个新线程
    // &tid1: 指向新线程标识符的指针
    // NULL: 线程属性,这里使用默认属性
    // thr_fn1: 新线程将要执行的函数
    // NULL: 传递给新线程函数的参数
    err = pthread_create(&tid1, NULL, thr_fn1, NULL);
    if (err != 0)
        err_exit(err, "can't create thread 1");  // 如果创建线程失败,调用err_exit函数处理错误

    // 创建线程2
    // 创建线程2
    // 函数:pthread_create
    // 参数:&tid2 - 线程ID的指针;NULL - 线程属性(使用默认属性);thr_fn2 - 线程执行的函数;NULL - 传递给线程函数的参数
    // 返回值:如果成功,返回0;否则返回错误代码
    err = pthread_create(&tid2, NULL, thr_fn2, NULL);
    // 检查是否成功创建线程
    if (err != 0)
        // 如果创建失败,调用err_exit函数处理错误,并输出错误信息"can't create thread 2"
        err_exit(err, "can't create thread 2");

    // 等待线程1结束并获取返回值
    // 等待线程1结束并获取其退出状态
    err = pthread_join(tid1, &tret);
    // 如果pthread_join函数调用失败,打印错误信息并退出程序
    if (err != 0)
        err_exit(err, "can't join with thread 1");
    // 打印线程1的退出代码
    printf("thread 1 exit code %ld\n", (long)tret);

    // 等待线程2结束并获取返回值
    // 等待线程2结束,并获取其退出状态
    err = pthread_join(tid2, &tret);
    // 如果pthread_join函数返回错误,则调用err_exit函数输出错误信息并退出程序
    if (err != 0)
        err_exit(err, "can't join with thread 2");
    // 打印线程2的退出代码
    printf("thread 2 exit code %ld\n", (long)tret);

    // 正常退出程序
    exit(0);
}
