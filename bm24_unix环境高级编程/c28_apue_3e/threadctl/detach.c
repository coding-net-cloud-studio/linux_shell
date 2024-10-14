#include "apue.h"
#include <pthread.h>

// 函数功能:创建一个线程,并设置为分离状态
// 参数:fn - 线程函数的指针;arg - 传递给线程函数的参数
// 返回值:成功返回0,失败返回错误码
int makethread(void *(*fn)(void *), void *arg)
{
    int            err;   // 错误码
    pthread_t      tid;   // 线程ID
    pthread_attr_t attr;  // 线程属性

    // 初始化线程属性
    err = pthread_attr_init(&attr);
    if (err != 0)
        return (err);

    // 设置线程属性为分离状态
    err = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    if (err == 0)
        // 创建线程
        err = pthread_create(&tid, &attr, fn, arg);

    // 销毁线程属性对象
    pthread_attr_destroy(&attr);

    // 返回错误码
    return (err);
}
