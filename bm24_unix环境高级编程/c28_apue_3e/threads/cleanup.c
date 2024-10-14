// cleanup.c

// 引入apue库的头文件,该库提供了一套用于UNIX和类UNIX系统的应用程序编程接口.
#include "apue.h"

// 引入POSIX线程库的头文件,用于实现多线程编程.
#include <pthread.h>

// 清理函数,用于在线程退出前执行清理工作
// arg: 传递给清理函数的参数
void cleanup(void *arg)
{
    printf("cleanup: %s\n", (char *)arg);
}

// 线程函数1
// arg: 传递给线程函数的参数
void *
thr_fn1(void *arg)
{
    printf("thread 1 start\n");  // 打印线程开始信息

    // 注册清理函数,当线程退出时执行
    // 第一个清理函数
    pthread_cleanup_push(cleanup, "thread 1 first handler");
    // 第二个清理函数
    pthread_cleanup_push(cleanup, "thread 1 second handler");

    printf("thread 1 push complete\n");  // 打印清理函数注册完成信息

    if (arg)                 // 如果参数非空
        return ((void *)1);  // 线程直接返回

    // 如果参数为空,执行清理函数并弹出
    // 清理函数调用,用于在pthread线程退出前执行清理工作
    // pthread_cleanup_pop函数的第二个参数为非0时,会执行清理函数
    // 这里两次调用pthread_cleanup_pop(0),意味着不会执行任何清理函数
    pthread_cleanup_pop(0);
    // 同上,不会执行任何清理函数
    pthread_cleanup_pop(0);

    return ((void *)1);  // 线程返回
}

void *
thr_fn2(void *arg)
{
    // 打印线程2开始的提示信息
    printf("线程2开始\n");

    // 注册线程清理函数,第一个处理程序
    // 注意:实际代码中应包含cleanup函数的定义
    pthread_cleanup_push(cleanup, "线程2第一个处理程序");

    // 注册线程清理函数,第二个处理程序
    pthread_cleanup_push(cleanup, "线程2第二个处理程序");

    // 打印线程2清理函数注册完成的提示信息
    printf("线程2清理函数注册完成\n");

    // 如果arg不为空,则线程退出并返回值2
    if (arg)
        pthread_exit((void *)2);

    // 弹出并执行第一个清理函数
    pthread_cleanup_pop(0);

    // 弹出并执行第二个清理函数
    pthread_cleanup_pop(0);

    // 线程退出并返回值2
    pthread_exit((void *)2);
}
// cleanup.c

/**
 * 主函数,创建并加入两个线程
 */
int main(void)
{
    int       err;         // 错误码
    pthread_t tid1, tid2;  // 线程标识符
    void     *tret;        // 线程返回值

    // 创建第一个线程
    // 创建线程1
    // 使用pthread_create函数创建一个新的线程,该线程将执行thr_fn1函数
    // 第一个参数是指向新线程标识符的指针
    // 第二个参数是线程属性,这里使用默认属性NULL
    // 第三个参数是要执行的函数的指针
    // 第四个参数是传递给该函数的参数,这里传递的是整数1的地址
    err = pthread_create(&tid1, NULL, thr_fn1, (void *)1);
    if (err != 0)
        // 如果线程创建失败,调用err_exit函数处理错误
        err_exit(err, "can't create thread 1");

    // 创建第二个线程
    // 创建线程2
    // pthread_create: 创建一个新线程
    // &tid2: 线程标识符的地址,用于存储新创建线程的ID
    // NULL: 使用默认线程属性
    // thr_fn2: 新线程将要执行的函数
    // (void *)1: 传递给新线程函数的参数
    err = pthread_create(&tid2, NULL, thr_fn2, (void *)1);
    if (err != 0)
        err_exit(err, "can't create thread 2");  // 如果线程创建失败,调用err_exit函数并退出

    // 等待第一个线程结束并获取返回值
    // 等待线程1结束,并获取其返回值
    err = pthread_join(tid1, &tret);
    // 如果等待失败,输出错误信息并退出程序
    if (err != 0)
        err_exit(err, "can't join with thread 1");
    // 打印线程1的退出代码
    printf("线程1退出代码 %ld\n", (long)tret);

    // 等待第二个线程结束并获取返回值
    // 等待线程2结束,并获取其退出码
    err = pthread_join(tid2, &tret);
    // 如果等待失败,打印错误信息并退出
    if (err != 0)
        err_exit(err, "can't join with thread 2");
    // 打印线程2的退出码
    printf("线程2退出码 %ld\n", (long)tret);

    // 正常退出程序
    exit(0);
}
