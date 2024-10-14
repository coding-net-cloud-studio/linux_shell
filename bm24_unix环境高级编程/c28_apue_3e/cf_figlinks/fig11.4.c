#include "apue.h"
#include <pthread.h>

// 定义结构体foo,包含四个整型成员变量
struct foo
{
    int a, b, c, d;  // 成员变量a, b, c, d
};

// 打印结构体foo的信息
// 参数s为要打印的字符串,fp为指向foo结构体的指针
void printfoo(const char *s, const struct foo *fp)
{
    printf("%s", s);                                      // 打印字符串s
    printf("  structure at 0x%lx\n", (unsigned long)fp);  // 打印结构体地址
    printf("  foo.a = %d\n", fp->a);                      // 打印成员变量a的值
    printf("  foo.b = %d\n", fp->b);                      // 打印成员变量b的值
    printf("  foo.c = %d\n", fp->c);                      // 打印成员变量c的值
    printf("  foo.d = %d\n", fp->d);                      // 打印成员变量d的值
}

// 线程函数thr_fn1
// 参数arg为线程函数的参数
void *
thr_fn1(void *arg)
{
    struct foo foo = {1, 2, 3, 4};  // 初始化结构体foo

    printfoo("thread 1:\n", &foo);  // 打印结构体foo的信息
    pthread_exit((void *)&foo);     // 线程退出,并返回指向foo结构体的指针
}

// 线程函数thr_fn2
// 参数arg为线程函数的参数
void *
thr_fn2(void *arg)
{
    printf("thread 2: ID is %lu\n", (unsigned long)pthread_self());  // 打印线程ID
    pthread_exit((void *)0);                                         // 线程退出,并返回NULL指针
}

int main(void)
{
    int         err;         // 错误码变量
    pthread_t   tid1, tid2;  // 线程标识符
    struct foo *fp;          // 指向foo结构体的指针

    // 创建第一个线程
    err = pthread_create(&tid1, NULL, thr_fn1, NULL);
    if (err != 0)  // 如果创建失败,退出程序
        err_exit(err, "can't create thread 1");

    // 等待第一个线程结束,并获取其返回的结构体指针
    err = pthread_join(tid1, (void *)&fp);
    if (err != 0)  // 如果等待失败,退出程序
        err_exit(err, "can't join with thread 1");

    sleep(1);                                   // 主线程休眠1秒
    printf("parent starting second thread\n");  // 打印信息

    // 创建第二个线程
    err = pthread_create(&tid2, NULL, thr_fn2, NULL);
    if (err != 0)  // 如果创建失败,退出程序
        err_exit(err, "can't create thread 2");

    sleep(1);                   // 主线程再次休眠1秒
    printfoo("parent:\n", fp);  // 打印foo结构体信息

    exit(0);  // 正常退出程序
}
