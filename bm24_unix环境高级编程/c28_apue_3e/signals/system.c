#include <sys/wait.h>  // 包含wait系统调用的头文件,用于等待子进程结束
#include <errno.h>     // 包含错误码定义的头文件,用于处理系统调用错误
#include <signal.h>    // 包含信号处理函数的头文件,用于处理异步事件
#include <unistd.h>    // 包含基本系统调用的头文件,如fork, exec等

// 注意:以上头文件均为系统编程中常用的头文件,用于进程控制,错误处理和信号处理等功能.
// 在编写系统级程序时,这些头文件提供了必要的接口和定义.

/**
 * @brief 执行一个shell命令
 *
 * @param cmdstring 要执行的shell命令字符串
 * @return int 命令执行的状态码,如果出错返回-1
 */
int system(const char *cmdstring) /* with appropriate signal handling */
{
    pid_t            pid;                         // 进程ID
    int              status;                      // 进程状态
    struct sigaction ignore, saveintr, savequit;  // 信号动作结构体
    sigset_t         chldmask, savemask;          // 信号集

    if (cmdstring == NULL)
        return (1); /* always a command processor with UNIX */

    // 忽略SIGINT和SIGQUIT信号
    // 忽略SIGINT信号的处理函数
    ignore.sa_handler = SIG_IGN;

    // 清空信号掩码集
    sigemptyset(&ignore.sa_mask);

    // 设置信号处理函数的标志位为0
    ignore.sa_flags = 0;

    // 设置SIGINT信号的处理函数为忽略,并保存原有处理函数到saveintr
    // 如果设置失败,返回-1
    if (sigaction(SIGINT, &ignore, &saveintr) < 0)
        return (-1);

    // 设置SIGQUIT信号的处理函数为忽略,并保存原有处理函数到savequit
    // 如果设置失败,返回-1
    if (sigaction(SIGQUIT, &ignore, &savequit) < 0)
        return (-1);

    // 阻塞SIGCHLD信号
    // 清空信号集chldmask
    sigemptyset(&chldmask);

    // 向信号集chldmask中添加SIGCHLD信号
    sigaddset(&chldmask, SIGCHLD);

    // 阻塞SIGCHLD信号,保存当前的信号掩码到savemask
    // 如果操作失败,返回-1
    // 系统调用:阻塞SIGCHLD信号
    if (sigprocmask(SIG_BLOCK, &chldmask, &savemask) < 0)
        return (-1);  // 阻塞失败返回-1

    // 创建子进程
    if ((pid = fork()) < 0)
    {
        status = -1;  // 进程创建失败
    }
    else if (pid == 0)
    { /* child */
        // 子进程执行的代码
        // 恢复之前的信号动作和信号掩码
        sigaction(SIGINT, &saveintr, NULL);         // 恢复SIGINT信号处理函数
        sigaction(SIGQUIT, &savequit, NULL);        // 恢复SIGQUIT信号处理函数
        sigprocmask(SIG_SETMASK, &savemask, NULL);  // 恢复信号掩码

        // 执行shell命令
        execl("/bin/sh", "sh", "-c", cmdstring, (char *)0);  // 执行cmdstring指定的shell命令
        _exit(127);                                          // 如果execl执行失败,退出子进程并返回错误码127
    }
    else
    { /* parent */
        // 父进程执行的代码
        // 等待子进程结束
        while (waitpid(pid, &status, 0) < 0)  // 等待子进程结束并获取其退出状态
            if (errno != EINTR)
            {
                status = -1;  // 如果等待过程中被信号中断且不是EINTR错误,则等待出错
                break;
            }
    }

    // 恢复之前的信号动作和信号掩码
    // 为SIGINT信号设置动作,saveintr为处理函数,如果设置失败返回-1
    if (sigaction(SIGINT, &saveintr, NULL) < 0)
        return (-1);

    // 为SIGQUIT信号设置动作,savequit为处理函数,如果设置失败返回-1
    if (sigaction(SIGQUIT, &savequit, NULL) < 0)
        return (-1);

    // 设置进程信号掩码为savemask,如果设置失败返回-1
    if (sigprocmask(SIG_SETMASK, &savemask, NULL) < 0)
        return (-1);

    return (status);  // 返回命令执行的状态码
}
