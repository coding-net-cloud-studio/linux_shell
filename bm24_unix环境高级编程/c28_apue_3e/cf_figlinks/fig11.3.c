#include "apue.h"
#include <pthread.h>

// 线程函数1
// 功能:打印线程1返回的信息,并返回一个指针值1
// 参数:arg - 线程参数
// 返回:指向整数1的指针
void *
thr_fn1(void *arg)
{
    printf("thread 1 returning\n");
    return ((void *)1);
}

// 线程函数2
// 功能:打印线程2退出的信息,并调用pthread_exit函数退出线程,返回一个指针值2
// 参数:arg - 线程参数
// 返回:无(pthread_exit函数的返回值不会被调用者获取)
void *
thr_fn2(void *arg)
{
    // 线程2退出
    // 该函数调用pthread_exit,用于终止当前线程的执行,并返回一个指针值.
    // 在这个例子中,返回的是一个指向整数2的指针,可以用于主线程或其他线程识别该线程的退出状态.
    printf("thread 2 exiting\n");
    pthread_exit((void *)2);
}
// fig11.3.c

/**
 * 主函数,创建并管理两个线程.
 */
int main(void)
{
    int       err;         // 错误码
    pthread_t tid1, tid2;  // 线程标识符
    void     *tret;        // 线程返回值指针

    // 创建第一个线程
    // 创建线程1
    // pthread_create函数用于创建一个新线程,成功返回0,失败返回错误码
    // &tid1: 指向新线程ID的指针
    // NULL: 线程属性,这里使用默认属性
    // thr_fn1: 新线程执行的函数
    // NULL: 传递给新线程函数的参数
    err = pthread_create(&tid1, NULL, thr_fn1, NULL);
    if (err != 0)
        // 如果创建线程失败,调用err_exit函数处理错误
        // err: 错误码
        // "can't create thread 1": 错误信息
        err_exit(err, "can't create thread 1");

    // 创建第二个线程
    // 创建线程2
    // pthread_create函数用于创建一个新线程,该线程将执行thr_fn2函数
    // &tid2: 线程标识符的地址,用于存储新创建线程的ID
    // NULL: 线程属性,这里使用默认属性
    // thr_fn2: 新线程将要执行的函数
    // NULL: 传递给新线程函数的参数,这里没有参数
    err = pthread_create(&tid2, NULL, thr_fn2, NULL);
    if (err != 0)
        err_exit(err, "can't create thread 2");  // 如果创建线程失败,调用err_exit函数处理错误

    // 等待第一个线程结束并获取返回值
    // 等待线程1结束,并获取其返回值
    err = pthread_join(tid1, &tret);
    // 如果等待失败,输出错误信息并退出
    if (err != 0)
        err_exit(err, "can't join with thread 1");
    // 打印线程1的退出代码
    printf("线程 1 退出代码 %ld\n", (long)tret);

    // 等待第二个线程结束并获取返回值
    // 等待线程2结束,并获取其退出码
    err = pthread_join(tid2, &tret);
    // 如果等待失败,输出错误信息并退出程序
    if (err != 0)
        err_exit(err, "can't join with thread 2");
    // 打印线程2的退出码
    printf("thread 2 exit code %ld\n", (long)tret);

    // 正常退出程序
    exit(0);
}
