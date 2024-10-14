#include "apue.h"          // 包含APUE库的头文件
#include <sys/resource.h>  // 包含系统资源限制相关的头文件

// 定义doit宏,用于打印指定资源的限制信息
#define doit(name) pr_limits(#name, name)

// 函数原型声明:打印资源限制信息
static void pr_limits(char *, int);
int         main(void)
{
    // 如果定义了 RLIMIT_AS,则调用 doit 函数并传入 RLIMIT_AS
#ifdef RLIMIT_AS
    doit(RLIMIT_AS);
#endif

    // 调用 doit 函数并传入不同的资源限制常量
    doit(RLIMIT_CORE);   // 核心文件大小限制
    doit(RLIMIT_CPU);    // CPU 时间限制
    doit(RLIMIT_DATA);   // 数据段大小限制
    doit(RLIMIT_FSIZE);  // 文件大小限制

#ifdef RLIMIT_MEMLOCK
    doit(RLIMIT_MEMLOCK);  // 内存锁定区域大小限制
#endif

#ifdef RLIMIT_MSGQUEUE
    doit(RLIMIT_MSGQUEUE);  // 消息队列大小限制
#endif

#ifdef RLIMIT_NICE
    doit(RLIMIT_NICE);  // 进程调度优先级限制
#endif

    doit(RLIMIT_NOFILE);  // 打开文件描述符数量限制

#ifdef RLIMIT_NPROC
    doit(RLIMIT_NPROC);  // 用户可拥有的进程数限制
#endif

#ifdef RLIMIT_NPTS
    doit(RLIMIT_NPTS);  // 用户可拥有的伪终端数限制
#endif

#ifdef RLIMIT_RSS
    doit(RLIMIT_RSS);  // 常驻集大小限制
#endif

#ifdef RLIMIT_SBSIZE
    doit(RLIMIT_SBSIZE);  // 套接字缓冲区大小限制
#endif

#ifdef RLIMIT_SIGPENDING
    doit(RLIMIT_SIGPENDING);  // 待处理信号数限制
#endif

    doit(RLIMIT_STACK);  // 栈大小限制

#ifdef RLIMIT_SWAP
    doit(RLIMIT_SWAP);  // 交换分区大小限制
#endif

#ifdef RLIMIT_VMEM
    doit(RLIMIT_VMEM);  // 虚拟内存大小限制
#endif

    exit(0);  // 程序正常退出
}

static void
pr_limits(char *name, int resource)  // 定义函数pr_limits,接收资源名称和资源类型作为参数
{
    struct rlimit      limit;  // 定义rlimit结构体变量limit,用于存储资源的限制信息
    unsigned long long lim;    // 定义无符号长整型变量lim,用于存储资源限制的具体数值

    if (getrlimit(resource, &limit) < 0)          // 调用getrlimit函数获取指定资源的限制信息,如果失败则返回错误
        err_sys("getrlimit error for %s", name);  // 如果获取资源限制失败,打印错误信息并退出程序
    printf("%-14s  ", name);                      // 打印资源名称,左对齐并占据14个字符宽度
    if (limit.rlim_cur == RLIM_INFINITY)
    {                            // 如果当前资源限制是无限大
        printf("(infinite)  ");  // 打印"(infinite)"表示无限大
    }
    else
    {
        lim = limit.rlim_cur;     // 否则,将当前资源限制值赋给lim
        printf("%10lld  ", lim);  // 打印当前资源限制值,右对齐并占据10个字符宽度
    }
    if (limit.rlim_max == RLIM_INFINITY)
    {                          // 如果最大资源限制是无限大
        printf("(infinite)");  // 打印"(infinite)"表示无限大
    }
    else
    {
        lim = limit.rlim_max;   // 否则,将最大资源限制值赋给lim
        printf("%10lld", lim);  // 打印最大资源限制值,右对齐并占据10个字符宽度
    }
    putchar((int)'\n');  // 打印换行符,结束当前行的输出
}
