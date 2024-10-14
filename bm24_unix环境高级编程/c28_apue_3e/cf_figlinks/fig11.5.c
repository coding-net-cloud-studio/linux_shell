#include "apue.h"     // 引入APUE库的头文件
#include <pthread.h>  // 引入POSIX线程库的头文件

// 清理函数,在pthread_exit()被调用或者线程被取消时执行
// @param arg 传递给清理函数的参数
void cleanup(void *arg)
{
    printf("cleanup: %s\n", (char *)arg);  // 打印清理信息
}

void *
thr_fn1(void *arg)  // 定义线程函数thr_fn1,接收一个void指针类型的参数
{
    printf("thread 1 start\n");                                // 打印线程1开始的提示信息
    pthread_cleanup_push(cleanup, "thread 1 first handler");   // 注册线程清理函数,第一个处理程序
    pthread_cleanup_push(cleanup, "thread 1 second handler");  // 注册线程清理函数,第二个处理程序
    printf("thread 1 push complete\n");                        // 打印线程1清理函数注册完成的提示信息
    if (arg)                                                   // 如果传入的参数不为空
        return ((void *)1);                                    // 直接返回,不再执行清理函数
    pthread_cleanup_pop(0);                                    // 弹出并执行第一个清理函数
    pthread_cleanup_pop(0);                                    // 弹出并执行第二个清理函数
    return ((void *)1);                                        // 返回
}

void *
thr_fn2(void *arg)  // 定义线程函数thr_fn2,参数为void指针类型
{
    printf("thread 2 start\n");  // 打印线程2开始的信息

    // 注册清理函数,当线程退出时执行.第一个清理处理程序的描述为"thread 2 first handler"
    pthread_cleanup_push(cleanup, "thread 2 first handler");
    // 注册第二个清理处理程序,描述为"thread 2 second handler"
    pthread_cleanup_push(cleanup, "thread 2 second handler");

    printf("thread 2 push complete\n");  // 打印清理函数注册完成的信息

    // 如果传入的参数arg不为空,则线程退出,并返回值2
    if (arg)
        pthread_exit((void *)2);

    // 如果arg为空,执行清理处理程序,0表示执行pop操作但不执行清理函数
    pthread_cleanup_pop(0);
    pthread_cleanup_pop(0);  // 再次执行清理处理程序

    pthread_exit((void *)2);  // 线程退出,并返回值2
}

int main(void)
{
    int       err;         // 错误码变量
    pthread_t tid1, tid2;  // 线程标识符
    void     *tret;        // 指向线程返回值的指针

    // 创建线程1
    err = pthread_create(&tid1, NULL, thr_fn1, (void *)1);
    if (err != 0)  // 如果创建失败,打印错误并退出
        err_exit(err, "can't create thread 1");

    // 创建线程2
    err = pthread_create(&tid2, NULL, thr_fn2, (void *)1);
    if (err != 0)  // 如果创建失败,打印错误并退出
        err_exit(err, "can't create thread 2");

    // 等待线程1结束,并获取返回值
    err = pthread_join(tid1, &tret);
    if (err != 0)  // 如果等待失败,打印错误并退出
        err_exit(err, "can't join with thread 1");
    printf("thread 1 exit code %ld\n", (long)tret);  // 打印线程1的退出码

    // 等待线程2结束,并获取返回值
    err = pthread_join(tid2, &tret);
    if (err != 0)  // 如果等待失败,打印错误并退出
        err_exit(err, "can't join with thread 2");
    printf("thread 2 exit code %ld\n", (long)tret);  // 打印线程2的退出码

    exit(0);  // 正常退出程序
}
