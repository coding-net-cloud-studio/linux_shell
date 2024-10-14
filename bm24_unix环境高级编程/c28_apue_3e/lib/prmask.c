#include "apue.h"   // 引入APUE库的头文件,该库提供了一套用于编写可移植UNIX应用程序的函数.
#include <errno.h>  // 引入标准错误处理头文件,包含了errno变量和相关的错误代码定义.

// 打印当前进程的信号屏蔽字
// @param str 描述信息,将打印在信号列表之前
void pr_mask(const char *str)
{
    sigset_t sigset;      // 用于存储信号屏蔽字
    int      errno_save;  // 保存调用前的errno值

    errno_save = errno;                     // 保存errno值,因为sigprocmask可能会改变它
    if (sigprocmask(0, NULL, &sigset) < 0)  // 获取当前进程的信号屏蔽字
    {
        err_ret("sigprocmask error");  // 如果出错,打印错误信息
    }
    else
    {
        printf("%s", str);  // 打印描述信息
        // 检查并打印特定信号是否在屏蔽字中
        // 检查信号集sigset中是否包含SIGINT信号,如果包含则打印" SIGINT"
        if (sigismember(&sigset, SIGINT))
            printf(" SIGINT");

        // 检查信号集sigset中是否包含SIGQUIT信号,如果包含则打印" SIGQUIT"
        if (sigismember(&sigset, SIGQUIT))
            printf(" SIGQUIT");

        // 检查信号集sigset中是否包含SIGUSR1信号,如果包含则打印" SIGUSR1"
        if (sigismember(&sigset, SIGUSR1))
            printf(" SIGUSR1");

        // 检查信号集sigset中是否包含SIGALRM信号,如果包含则打印" SIGALRM"
        if (sigismember(&sigset, SIGALRM))
            printf(" SIGALRM");

        /* remaining signals can go here  */

        printf("\n");  // 打印换行符
    }

    errno = errno_save;  // 恢复调用前的errno值
}
