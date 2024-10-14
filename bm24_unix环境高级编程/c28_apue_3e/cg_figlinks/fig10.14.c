#include "apue.h"   // 包含APUE库的头文件,该库提供了一套用于编写可移植UNIX应用程序的函数.
#include <errno.h>  // 包含定义错误码的头文件,这些错误码在系统调用失败时返回.

// 打印当前进程的信号屏蔽字
// @param str 打印信息的前缀
void pr_mask(const char *str)
{
    sigset_t sigset;      // 用于存储信号屏蔽字的变量
    int      errno_save;  // 保存调用前的errno值,因为sigprocmask可能会修改它

    errno_save = errno;                     // 保存当前errno值
    if (sigprocmask(0, NULL, &sigset) < 0)  // 获取当前进程的信号屏蔽字
    {
        err_ret("sigprocmask error");  // 如果出错,打印错误信息
    }
    else
    {
        printf("%s", str);                 // 打印前缀信息
        if (sigismember(&sigset, SIGINT))  // 检查SIGINT信号是否在屏蔽字中
            printf(" SIGINT");
        if (sigismember(&sigset, SIGQUIT))  // 检查SIGQUIT信号是否在屏蔽字中
            printf(" SIGQUIT");
        if (sigismember(&sigset, SIGUSR1))  // 检查SIGUSR1信号是否在屏蔽字中
            printf(" SIGUSR1");
        if (sigismember(&sigset, SIGALRM))  // 检查SIGALRM信号是否在屏蔽字中
            printf(" SIGALRM");

        /* remaining signals can go here  */  // 其他信号可以在这里添加

        printf("\n");  // 打印换行符
    }

    errno = errno_save;  // 恢复原来的errno值
}
