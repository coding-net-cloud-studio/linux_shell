#include "apue.h"      // 引入apue库的头文件
#include <errno.h>     // 引入错误处理头文件
#include <sys/time.h>  // 引入系统时间头文件

// 根据不同的操作系统包含不同的头文件
#if defined(MACOS)
#include <sys/syslimits.h>  // MacOS系统特定的头文件
#elif defined(SOLARIS)
#include <limits.h>  // Solaris系统特定的头文件
#elif defined(BSD)
#include <sys/param.h>  // BSD系统特定的头文件
#endif

unsigned long long count;  // 定义一个无符号长整型变量count,用于计数
struct timeval     end;    // 定义一个结构体变量end,用于存储结束时间
// checktime函数用于检查当前时间是否已经超过了预设的结束时间.
// 如果当前时间大于或等于结束时间,它将打印出传入的字符串和计数器的值,并退出程序.
//
// @param str 需要打印的字符串
void checktime(char *str)
{
    struct timeval tv;  // 定义一个timeval结构体用于存储当前时间

    gettimeofday(&tv, NULL);  // 获取当前时间并存储到tv中
    // 比较当前时间和结束时间,如果当前时间大于或等于结束时间
    if (tv.tv_sec >= end.tv_sec && tv.tv_usec >= end.tv_usec)
    {
        printf("%s count = %lld\n", str, count);  // 打印字符串和计数器的值
        exit(0);                                  // 退出程序
    }
}

int main(int argc, char *argv[])
{
    pid_t pid;         // 进程ID
    char *s;           // 指向字符串的指针
    int   nzero, ret;  // nzero用于存储NZERO的值,ret用于存储nice函数的返回值
    int   adj = 0;     // 调整nice值的变量

    setbuf(stdout, NULL);  // 设置标准输出的缓冲区

#if defined(NZERO)
    nzero = NZERO;  // 如果定义了NZERO,则使用NZERO的值
#elif defined(_SC_NZERO)
    nzero = sysconf(_SC_NZERO);  // 如果定义了_SC_NZERO,则使用sysconf获取NZERO的值
#else
#error NZERO undefined // 如果都没有定义,则报错
#endif

    printf("NZERO = %d\n", nzero);  // 打印NZERO的值

    if (argc == 2)                        // 如果命令行参数有2个(包括程序名)
        adj = strtol(argv[1], NULL, 10);  // 将第二个参数转换为整数并赋值给adj

    gettimeofday(&end, NULL);  // 获取当前时间
    end.tv_sec += 10;          // 设置程序运行时间为10秒

    if ((pid = fork()) < 0)
    {                            // 创建子进程
        err_sys("fork failed");  // 如果fork失败,则报错
    }
    else if (pid == 0)
    {  // 子进程
        s = "child";
        printf("current nice value in child is %d, adjusting by %d\n",
               nice(0) + nzero,                               // 打印当前nice值并加上NZERO的值
               adj);                                          // 打印调整值
        errno = 0;                                            // 清除errno
        if ((ret = nice(adj)) == -1 && errno != 0)            // 尝试调整nice值
            err_sys("child set scheduling priority");         // 如果失败,则报错
        printf("now child nice value is %d\n", ret + nzero);  // 打印调整后的nice值
    }
    else
    {  // 父进程
        s = "parent";
        printf("current nice value in parent is %d\n", nice(0) + nzero);  // 打印当前nice值并加上NZERO的值
    }

    for (;;)
    {                                        // 无限循环
        if (++count == 0)                    // 计数器递增
            err_quit("%s counter wrap", s);  // 如果计数器溢出,则报错
        checktime(s);                        // 检查时间
    }
}
