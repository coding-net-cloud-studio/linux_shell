#include "apue.h"     // 引入APUE库,提供系统调用和C标准库函数的封装
#include <pthread.h>  // 引入POSIX线程库,用于多线程编程

/**
 * @brief 创建一个新线程并立即分离它.
 *
 * 这个函数用于创建一个新的线程,并将其设置为分离状态,这意味着线程结束时会自动清理资源,而不需要其他线程来连接(join)它.
 *
 * @param fn 指向线程函数的指针.
 * @param arg 传递给线程函数的参数.
 * @return 如果成功创建并分离线程,返回0;如果发生错误,返回错误代码.
 */
int makethread(void *(*fn)(void *), void *arg)
{
    int            err;   // 错误码
    pthread_t      tid;   // 线程标识符
    pthread_attr_t attr;  // 线程属性对象

    // 初始化线程属性对象
    // 初始化线程属性
    // 函数:pthread_attr_init
    // 功能:初始化一个线程属性对象,用于后续创建线程时指定线程的属性
    // 参数:&attr - 指向pthread_attr_t类型的指针,用于存储初始化后的线程属性
    // 返回值:成功返回0,失败返回错误码
    err = pthread_attr_init(&attr);
    if (err != 0)
    {
        return (err);  // 初始化失败,返回错误码
    }

    // 设置线程属性为分离状态
    // 设置线程属性为分离状态,这样创建的线程在终止时会自动清理资源
    err = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    if (err == 0)
    {
        // 如果设置属性成功,则创建新线程
        // tid: 线程标识符
        // &attr: 线程属性
        // fn: 线程执行的函数
        // arg: 传递给线程函数的参数
        err = pthread_create(&tid, &attr, fn, arg);
    }

    // 销毁线程属性对象
    pthread_attr_destroy(&attr);
    return (err);  // 返回创建线程的结果
}
