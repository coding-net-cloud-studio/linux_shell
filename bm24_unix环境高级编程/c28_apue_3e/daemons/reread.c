#include "apue.h"     // 引入APUE库的头文件,该库提供了一套用于编写可移植UNIX应用程序的函数.
#include <pthread.h>  // 引入POSIX线程库的头文件,用于创建和管理线程.
#include <syslog.h>   // 引入系统日志库的头文件,用于记录系统消息.

sigset_t mask;  // 定义信号集

// 检查守护进程是否已经在运行
extern int already_running(void);

// 重新读取配置文件的函数
void reread(void)
{
    /* ... */
}

// 线程函数,用于处理信号
void *
thr_fn(void *arg)
{
    int err, signo;

    // 无限循环等待信号
    for (;;)
    {
        // 等待信号
        err = sigwait(&mask, &signo);
        if (err != 0)
        {
            // 如果sigwait失败,记录错误日志并退出线程
            syslog(LOG_ERR, "sigwait failed");
            exit(1);
        }

        // 根据接收到的信号执行不同的操作
        switch (signo)
        {
        case SIGHUP:
            // 接收到SIGHUP信号,记录日志并重新读取配置文件
            syslog(LOG_INFO, "Re-reading configuration file");
            reread();
            break;

        case SIGTERM:
            // 接收到SIGTERM信号,记录日志并退出线程
            syslog(LOG_INFO, "got SIGTERM; exiting");
            exit(0);

        default:
            // 记录未预期的信号
            syslog(LOG_INFO, "unexpected signal %d\n", signo);
        }
    }
    return (0);
}

int main(int argc, char *argv[])
{
    // reread.c

    // 定义整型变量err,用于存储错误码
    int err;

    // 定义pthread_t类型的变量tid,用于存储线程ID
    pthread_t tid;

    // 定义字符指针cmd,用于指向命令字符串
    char *cmd;

    // 定义结构体sigaction类型的变量sa,用于存储信号处理动作
    struct sigaction sa;

    // 获取守护进程的名称
    if ((cmd = strrchr(argv[0], '/')) == NULL)
        cmd = argv[0];
    else
        cmd++;

    /*
     * Become a daemon.
     */
    daemonize(cmd);  // 变成守护进程

    /*
     * Make sure only one copy of the daemon is running.
     */
    if (already_running())
    {
        syslog(LOG_ERR, "daemon already running");  // 记录错误日志
        exit(1);                                    // 退出程序
    }

    /*
     * Restore SIGHUP default and block all signals.
     */
    sa.sa_handler = SIG_DFL;   // 恢复SIGHUP的默认处理
    sigemptyset(&sa.sa_mask);  // 清空信号掩码
    sa.sa_flags = 0;
    if (sigaction(SIGHUP, &sa, NULL) < 0)  // 设置SIGHUP信号处理
        err_quit("%s: can't restore SIGHUP default");
    sigfillset(&mask);                                         // 填充信号集
    if ((err = pthread_sigmask(SIG_BLOCK, &mask, NULL)) != 0)  // 阻塞所有信号
        err_exit(err, "SIG_BLOCK error");

    /*
     * Create a thread to handle SIGHUP and SIGTERM.
     */
    err = pthread_create(&tid, NULL, thr_fn, 0);  // 创建线程处理信号
    if (err != 0)
        err_exit(err, "can't create thread");

    /*
     * Proceed with the rest of the daemon.
     */
    /* ... */
    exit(0);
}
