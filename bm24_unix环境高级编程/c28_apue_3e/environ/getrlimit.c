#include "apue.h"
#include <sys/resource.h>

// 定义doit宏,用于打印资源限制信息
#define doit(name) pr_limits(#name, name)

// 函数原型声明
static void pr_limits(char *, int);

int main(void)
{
    // 根据不同的系统定义,打印对应的资源限制信息
#ifdef RLIMIT_AS
    doit(RLIMIT_AS);  // 地址空间限制
#endif

    doit(RLIMIT_CORE);   // 核心文件大小限制
    doit(RLIMIT_CPU);    // CPU时间限制
    doit(RLIMIT_DATA);   // 数据段大小限制
    doit(RLIMIT_FSIZE);  // 文件大小限制

#ifdef RLIMIT_MEMLOCK
    doit(RLIMIT_MEMLOCK);  // 内存锁定限制
#endif

#ifdef RLIMIT_MSGQUEUE
    doit(RLIMIT_MSGQUEUE);  // 消息队列限制
#endif

#ifdef RLIMIT_NICE
    doit(RLIMIT_NICE);  // 进程优先级限制
#endif

    doit(RLIMIT_NOFILE);  // 打开文件描述符数量限制

#ifdef RLIMIT_NPROC
    doit(RLIMIT_NPROC);  // 进程数限制
#endif

#ifdef RLIMIT_NPTS
    doit(RLIMIT_NPTS);  // TTY命名空间中伪终端的最大数量限制
#endif

#ifdef RLIMIT_RSS
    doit(RLIMIT_RSS);  // 常驻集大小限制
#endif

#ifdef RLIMIT_SBSIZE
    doit(RLIMIT_SBSIZE);  // 套接字缓冲区大小限制
#endif

#ifdef RLIMIT_SIGPENDING
    doit(RLIMIT_SIGPENDING);  // 待处理信号的数量限制
#endif

    doit(RLIMIT_STACK);  // 栈大小限制

#ifdef RLIMIT_SWAP
    doit(RLIMIT_SWAP);  // 交换分区使用量限制
#endif

#ifdef RLIMIT_VMEM
    doit(RLIMIT_VMEM);  // 虚拟内存大小限制
#endif

    exit(0);
}

// 打印资源限制信息的函数
static void
pr_limits(char *name, int resource)
{
    struct rlimit      limit;  // rlimit结构体,用于存储资源限制信息
    unsigned long long lim;    // 用于存储资源限制的值

    // 获取资源限制信息
    if (getrlimit(resource, &limit) < 0)
        err_sys("getrlimit error for %s", name);  // 如果出错,打印错误信息并退出
    printf("%-14s  ", name);                      // 打印资源名称
    // 打印当前资源限制值
    // 检查资源限制的当前值是否为无限
    if (limit.rlim_cur == RLIM_INFINITY)
    {
        // 如果是无限,打印"(infinite)"
        printf("(infinite)  ");
    }
    else
    {
        // 如果不是无限,将当前资源限制值赋给变量lim
        lim = limit.rlim_cur;
        // 打印资源限制值,格式化为10位宽,左对齐,后面跟两个空格
        printf("%10lld  ", lim);
    }

    // 打印最大资源限制值
    // 检查并打印资源限制的最大值
    if (limit.rlim_max == RLIM_INFINITY)
    {
        // 如果资源限制的最大值是无限大,则打印"(infinite)"
        printf("(infinite)");
    }
    else
    {
        // 否则,将资源限制的最大值赋给变量lim,并打印出来
        long long lim = limit.rlim_max;
        printf("%10lld", lim);
    }

    putchar((int)'\n');  // 换行
}
