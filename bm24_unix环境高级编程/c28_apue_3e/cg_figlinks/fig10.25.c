#include <signal.h>  // 包含信号处理函数的头文件
#include <stdio.h>   // 包含标准输入输出函数的头文件
#include <stdlib.h>  // 包含标准库函数的头文件
#include <unistd.h>  // 包含Unix系统调用函数的头文件

// @brief POSIX风格的abort()函数,用于异常退出程序
void abort(void)
{
    sigset_t         mask;    // 信号集,用于管理信号屏蔽
    struct sigaction action;  // 信号动作结构体,用于设置信号处理函数

    // 如果调用者忽略了SIGABRT信号,则将其重置为默认处理
    sigaction(SIGABRT, NULL, &action);
    if (action.sa_handler == SIG_IGN)
    {
        action.sa_handler = SIG_DFL;
        sigaction(SIGABRT, &action, NULL);
    }
    // 如果SIGABRT的处理函数是默认的,则刷新所有打开的标准IO流
    if (action.sa_handler == SIG_DFL)
        fflush(NULL);

    // 调用者不能阻塞SIGABRT信号;确保它没有被阻塞
    sigfillset(&mask);                      // 将mask信号集填充为所有信号
    sigdelset(&mask, SIGABRT);              // 从mask中删除SIGABRT信号,即只有SIGABRT信号不被屏蔽
    sigprocmask(SIG_SETMASK, &mask, NULL);  // 设置进程的信号屏蔽字
    kill(getpid(), SIGABRT);                // 向进程自身发送SIGABRT信号

    // 如果我们在这里,说明进程捕获了SIGABRT信号并返回
    fflush(NULL);                           // 再次刷新所有打开的标准IO流
    action.sa_handler = SIG_DFL;            // 重置SIGABRT的处理函数为默认
    sigaction(SIGABRT, &action, NULL);      // 重置信号处理函数
    sigprocmask(SIG_SETMASK, &mask, NULL);  // 重新设置信号屏蔽字,以防万一
    kill(getpid(), SIGABRT);                // 再次发送SIGABRT信号
    exit(1);                                // 这行代码不应该被执行到
}
