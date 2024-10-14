#include "apue.h"
#include <pthread.h>

// 定义一个结构体foo,包含四个整型成员变量
struct foo
{
    int a, b, c, d;  // 成员变量a, b, c, d
};

// 函数printfoo用于打印结构体foo的信息
// 参数s为要打印的字符串,fp为指向结构体foo的指针
void printfoo(const char *s, const struct foo *fp)
{
    printf("%s", s);                                      // 打印传入的字符串s
    printf("  structure at 0x%lx\n", (unsigned long)fp);  // 打印结构体指针fp的地址
    printf("  foo.a = %d\n", fp->a);                      // 打印结构体成员变量a的值
    printf("  foo.b = %d\n", fp->b);                      // 打印结构体成员变量b的值
    printf("  foo.c = %d\n", fp->c);                      // 打印结构体成员变量c的值
    printf("  foo.d = %d\n", fp->d);                      // 打印结构体成员变量d的值
}

// thr_fn1 线程函数1
// 功能:初始化一个foo结构体实例,并打印其内容,然后线程退出
// 参数:arg - 线程参数,本例中未使用
// 返回值:指向foo结构体的指针,在pthread_exit中传递
void *
thr_fn1(void *arg)
{
    // 初始化foo结构体实例
    struct foo foo = {1, 2, 3, 4};

    // 打印foo结构体的内容
    printfoo("thread 1:\n", &foo);

    // 线程退出,返回foo结构体的地址
    pthread_exit((void *)&foo);
}

// thr_fn2 线程函数2
// 功能:打印当前线程的ID,然后线程退出
// 参数:arg - 线程参数,本例中未使用
// 返回值:NULL,表示线程正常退出
void *
thr_fn2(void *arg)
{
    // 打印当前线程的ID
    printf("thread 2: ID is %lu\n", (unsigned long)pthread_self());

    // 线程退出,返回NULL
    pthread_exit((void *)0);
}

// main.c

/**
 * 主函数,创建并管理两个线程
 */
int main(void)
{
    int         err;         // 错误码
    pthread_t   tid1, tid2;  // 线程标识符
    struct foo *fp;          // 指向foo结构体的指针

    // 创建第一个线程
    err = pthread_create(&tid1, NULL, thr_fn1, NULL);
    if (err != 0)  // 如果创建失败,调用err_exit函数处理错误
        err_exit(err, "can't create thread 1");

    // 等待第一个线程结束,并获取其返回的foo结构体指针
    err = pthread_join(tid1, (void *)&fp);
    if (err != 0)  // 如果等待失败,调用err_exit函数处理错误
        err_exit(err, "can't join with thread 1");

    sleep(1);  // 主线程休眠1秒

    printf("parent starting second thread\n");  // 打印信息

    // 创建第二个线程
    err = pthread_create(&tid2, NULL, thr_fn2, NULL);
    if (err != 0)  // 如果创建失败,调用err_exit函数处理错误
        err_exit(err, "can't create thread 2");

    sleep(1);  // 主线程休眠1秒

    printfoo("parent:\n", fp);  // 打印foo结构体信息

    exit(0);  // 正常退出程序
}
