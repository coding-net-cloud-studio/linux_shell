#include <sys/wait.h>  // 包含wait系统调用的头文件,用于等待子进程结束
#include <errno.h>     // 包含错误码定义的头文件,用于处理系统调用错误
#include <signal.h>    // 包含信号处理函数的头文件,用于处理异步事件
#include <unistd.h>    // 包含unistd.h头文件,提供对POSIX操作系统API的访问

// system函数用于执行shell命令,支持信号处理
int system(const char *cmdstring) /* with appropriate signal handling */
{
    pid_t            pid;                         // 进程ID
    int              status;                      // 进程状态
    struct sigaction ignore, saveintr, savequit;  // 信号动作结构体
    sigset_t         chldmask, savemask;          // 信号集

    if (cmdstring == NULL)
        return (1);  // 如果命令字符串为空,返回1

    // 忽略SIGINT和SIGQUIT信号
    // 设置SIGINT信号的处理方式为忽略
    ignore.sa_handler = SIG_IGN;
    // 清空信号掩码集,表示处理该信号时不阻塞其他信号
    sigemptyset(&ignore.sa_mask);
    // 设置信号处理函数的标志位为0,使用默认行为
    ignore.sa_flags = 0;
    // 安装SIGINT信号的处理函数,如果失败返回-1
    if (sigaction(SIGINT, &ignore, &saveintr) < 0)
        return (-1);
    // 安装SIGQUIT信号的处理函数,如果失败返回-1
    if (sigaction(SIGQUIT, &ignore, &savequit) < 0)
        return (-1);

    sigemptyset(&chldmask);         // 清空chldmask信号集
    sigaddset(&chldmask, SIGCHLD);  // 在chldmask信号集中添加SIGCHLD信号
    if (sigprocmask(SIG_BLOCK, &chldmask, &savemask) < 0)
        return (-1);  // 阻塞SIGCHLD信号

    if ((pid = fork()) < 0)
    {
        status = -1;  // 如果fork失败,返回-1
    }
    else if (pid == 0)
    { /* child */
        // 子进程恢复之前的信号动作并重置信号掩码
        // 设置SIGINT信号的处理函数为saveintr,当用户按下Ctrl+C时触发
        sigaction(SIGINT, &saveintr, NULL);

        // 设置SIGQUIT信号的处理函数为savequit,当用户按下Ctrl+\时触发
        sigaction(SIGQUIT, &savequit, NULL);

        // 设置进程的信号屏蔽字为savemask,屏蔽某些信号
        sigprocmask(SIG_SETMASK, &savemask, NULL);

        // 执行shell命令
        // execl函数用于替换当前进程的镜像,执行/bin/sh
        // 第一个参数是可执行文件的路径,第二个参数是传递给新程序的argv[0],后续参数是新程序的命令行参数,最后一个参数必须是NULL
        execl("/bin/sh", "sh", "-c", cmdstring, (char *)0);

        _exit(127);  // 如果exec失败,退出子进程
    }
    else
    { /* parent */
        // 父进程等待子进程结束
        // 等待子进程结束
        while (waitpid(pid, &status, 0) < 0)
            // 如果waitpid函数返回错误,并且错误不是EINTR(系统调用被信号中断)
            if (errno != EINTR)
            {
                status = -1;  // 设置状态为-1,表示出现了非EINTR的错误
                break;        // 跳出循环
            }
    }

    // 恢复之前的信号动作并重置信号掩码
    if (sigaction(SIGINT, &saveintr, NULL) < 0)
        return (-1);
    if (sigfield(SIGQUIT, &savequit, NULL) < 0)
        return (-1);
    if (sigprocmask(SIG_SETMASK, &savemask, NULL) < 0)
        return (-1);

    return (status);  // 返回子进程的状态
}
