// nice.c

// 包含APUE库的头文件,APUE是"Applied Programming in the UNIX Environment"一书的缩写
#include "apue.h"

// 包含errno头文件,用于处理系统调用的错误代码
#include <errno.h>

// 包含sys/time.h头文件,提供访问系统时间函数的接口
#include <sys/time.h>

// 根据不同的操作系统定义,包含相应的头文件
#if defined(MACOS)
// MacOS系统特有的头文件,定义了系统限制
#include <sys/syslimits.h>
#elif defined(SOLARIS)
// Solaris系统特有的头文件,定义了各种限制
#include <limits.h>
#elif defined(BSD)
// BSD系统特有的头文件,包含了系统参数的定义
#include <sys/param.h>
#endif

// 注释结束

unsigned long long count;  // 计数器
struct timeval     end;    // 结束时间

// 检查是否到达结束时间,并打印计数器的值
void checktime(char *str)
{
    struct timeval tv;

    gettimeofday(&tv, NULL);  // 获取当前时间
    // 如果当前时间大于等于结束时间,则打印计数器的值并退出程序
    // 检查当前时间是否已经达到或超过了设定的结束时间
    // 如果当前时间的秒数和微秒数都大于或等于结束时间的对应值,则打印出字符串和计数器的值,并退出程序
    if (tv.tv_sec >= end.tv_sec && tv.tv_usec >= end.tv_usec)
    {
        // 打印字符串和计数器的值
        printf("%s count = %lld\n", str, count);
        // 退出程序,返回状态码0表示正常退出
        exit(0);
    }
}

int main(int argc, char *argv[])
{
    pid_t pid;         // 进程ID
    char *s;           // 用于存储"child"或"parent"
    int   nzero, ret;  // nzero是NZERO的值,ret是nice函数的返回值
    int   adj = 0;     // 调整nice值的参数

    setbuf(stdout, NULL);  // 设置stdout为无缓冲

    // 获取NZERO的值
#if defined(NZERO)
    nzero = NZERO;
#elif defined(_SC_NZERO)
    nzero = sysconf(_SC_NZERO);
#else
#error NZERO undefined
#endif
    printf("NZERO = %d\n", nzero);

    // 如果命令行参数个数为2,则将第二个参数转换为整数并赋值给adj
    if (argc == 2)
        adj = strtol(argv[1], NULL, 10);

    gettimeofday(&end, NULL);  // 获取当前时间作为结束时间
    end.tv_sec += 10;          // 设置程序运行时间为10秒

    // 创建子进程
    if ((pid = fork()) < 0)
    {
        err_sys("fork failed");  // 如果fork失败,则打印错误信息并退出
    }
    else if (pid == 0)
    { /* child */
        s = "child";
        // 打印子进程当前的nice值,并尝试调整nice值
        printf("current nice value in child is %d, adjusting by %d\n",
               nice(0) + nzero,
               adj);
        errno = 0;
        // 调整nice值,如果失败则打印错误信息并退出
        if ((ret = nice(adj)) == -1 && errno != 0)
            err_sys("child set scheduling priority");
        // 打印调整后的nice值
        printf("now child nice value is %d\n", ret + nzero);
    }
    else
    { /* parent */
        s = "parent";
        // 打印父进程当前的nice值
        printf("current nice value in parent is %d\n", nice(0) + nzero);
    }

    // 无限循环,每次循环计数器加1,并检查是否到达结束时间
    for (;;)
    {
        if (++count == 0)
            err_quit("%s counter wrap", s);  // 如果计数器溢出,则打印错误信息并退出
        checktime(s);                        // 检查是否到达结束时间
    }
}
