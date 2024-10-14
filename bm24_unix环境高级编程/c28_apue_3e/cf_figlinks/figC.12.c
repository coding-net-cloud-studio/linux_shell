#include "apue.h"     // 引入apue库,提供系统调用和工具函数的封装
#include <pthread.h>  // 引入POSIX线程库,用于多线程编程

// 定义一个结构体 foo,包含四个整数成员 a, b, c, d
// 该结构体可能用于存储线程间的共享数据或者作为线程函数的参数
struct foo
{
    int a, b, c, d;  // 成员变量 a, b, c, d 用于存储整数值
};

/**
 * @brief 打印结构体 foo 的内容
 *
 * @param s 前缀字符串
 * @param fp 指向结构体 foo 的指针
 */
void printfoo(const char *s, const struct foo *fp)
{
    // 将前缀字符串 s 写入标准输出
    fputs(s, stdout);
    // 打印结构体的内存地址
    printf("  structure at 0x%lx\n", (unsigned long)fp);
    // 打印结构体成员 a 的值
    printf("  foo.a = %d\n", fp->a);
    // 打印结构体成员 b 的值
    printf("  foo.b = %d\n", fp->b);
    // 打印结构体成员 c 的值
    printf("  foo.c = %d\n", fp->c);
    // 打印结构体成员 d 的值
    printf("  foo.d = %d\n", fp->d);
}

/**
 * @brief 线程函数,用于初始化结构体 foo 并打印其内容
 *
 * @param arg 线程函数的参数,未使用
 * @return void* 返回指向结构体 foo 的指针
 */
void *
thr_fn1(void *arg)
{
    // 定义一个指向结构体 foo 的指针 fp
    struct foo *fp;

    // 分配一个结构体 foo 大小的内存空间,并将其地址赋给 fp
    if ((fp = malloc(sizeof(struct foo))) == NULL)
        // 如果内存分配失败,调用 err_sys 函数输出错误信息并终止程序
        err_sys("can't allocate memory");
    // 初始化结构体成员 a 的值为 1
    fp->a = 1;
    // 初始化结构体成员 b 的值为 2
    fp->b = 2;
    // 初始化结构体成员 c 的值为 3
    fp->c = 3;
    // 初始化结构体成员 d 的值为 4
    fp->d = 4;
    // 调用 printfoo 函数打印结构体 foo 的内容
    printfoo("thread:\n", fp);
    // 返回指向结构体 foo 的指针
    return ((void *)fp);
}

/**
 * @brief 主函数,程序的入口点
 *
 * @return int 程序的退出状态码
 */
int main(void)
{
    int err;
    // 定义一个 pthread_t 类型的变量 tid1,用于存储线程 ID
    pthread_t tid1;
    // 定义一个指向结构体 foo 的指针 fp,用于存储线程返回的结构体指针
    struct foo *fp;

    // 创建一个新线程,线程 ID 存储在 tid1 中,线程函数为 thr_fn1,参数为 NULL
    err = pthread_create(&tid1, NULL, thr_fn1, NULL);
    // 如果线程创建失败,调用 err_exit 函数输出错误信息并终止程序
    if (err != 0)
        err_exit(err, "can't create thread 1");
    // 等待线程 tid1 结束,并将返回的结构体指针存储在 fp 中
    err = pthread_join(tid1, (void *)&fp);
    // 如果线程等待失败,调用 err_exit 函数输出错误信息并终止程序
    if (err != 0)
        err_exit(err, "can't join with thread 1");
    // 调用 printfoo 函数打印结构体 foo 的内容
    printfoo("parent:\n", fp);
    // 主函数正常结束,返回 0
    exit(0);
}
