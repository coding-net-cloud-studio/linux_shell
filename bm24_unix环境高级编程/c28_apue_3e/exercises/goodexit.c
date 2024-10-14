#include "apue.h"
#include <pthread.h>

// 定义一个结构体foo,包含四个整型成员变量
struct foo
{
    int a, b, c, d;  // 成员变量a, b, c, d
};

// 函数printfoo用于打印结构体foo的内容
// 参数s为要打印的字符串,fp为指向foo结构体的指针
void printfoo(const char *s, const struct foo *fp)
{
    fputs(s, stdout);                                     // 打印字符串s
    printf("  structure at 0x%lx\n", (unsigned long)fp);  // 打印结构体fp的地址
    printf("  foo.a = %d\n", fp->a);                      // 打印结构体成员变量a的值
    printf("  foo.b = %d\n", fp->b);                      // 打印结构体成员变量b的值
    printf("  foo.c = %d\n", fp->c);                      // 打印结构体成员变量c的值
    printf("  foo.d = %d\n", fp->d);                      // 打印结构体成员变量d的值
}

void *
thr_fn1(void *arg)  // 定义线程函数thr_fn1,参数为void指针
{
    struct foo *fp;  // 定义指向结构体foo的指针fp

    if ((fp = malloc(sizeof(struct foo))) == NULL)  // 尝试为结构体foo分配内存
        err_sys("can't allocate memory");           // 如果分配失败,调用err_sys函数输出错误信息并退出

    fp->a = 1;  // 初始化结构体成员a为1
    fp->b = 2;  // 初始化结构体成员b为2
    fp->c = 3;  // 初始化结构体成员c为3
    fp->d = 4;  // 初始化结构体成员d为4

    printfoo("thread:\n", fp);  // 调用printfoo函数打印结构体内容

    return ((void *)fp);  // 返回指向结构体的指针
}

/**
 * 主函数,创建并加入一个线程,然后打印线程中的数据结构.
 */
int main(void)
{
    int         err;   // 错误码
    pthread_t   tid1;  // 线程标识符
    struct foo *fp;    // 指向foo结构体的指针

    // 创建线程tid1,执行函数thr_fn1,不需要传递参数
    err = pthread_create(&tid1, NULL, thr_fn1, NULL);
    if (err != 0)                                // 如果创建线程失败
        err_exit(err, "can't create thread 1");  // 输出错误信息并退出程序

    // 等待线程tid1结束,并获取其返回值
    err = pthread_join(tid1, (void *)&fp);
    if (err != 0)                                   // 如果加入线程失败
        err_exit(err, "can't join with thread 1");  // 输出错误信息并退出程序

    printfoo("parent:\n", fp);  // 打印线程中的数据结构
    exit(0);                    // 正常退出程序
}
