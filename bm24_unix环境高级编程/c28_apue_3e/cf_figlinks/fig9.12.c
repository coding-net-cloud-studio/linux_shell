#include "apue.h"   // 引入apue库,提供系统调用和标准C库函数的封装
#include <errno.h>  // 引入errno头文件,用于访问系统调用的错误代码

// 定义信号处理函数sig_hup,当接收到SIGHUP信号时执行打印信息操作
// 参数: signo - 接收到的信号编号
static void
sig_hup(int signo)
{
    // 打印接收到的信号信息及当前进程ID
    printf("SIGHUP received, pid = %ld\n", (long)getpid());
}

// 定义函数pr_ids,用于打印当前进程的ID,父进程ID,进程组ID和终端进程组ID
// 参数: name - 进程名称,用于输出信息
static void
pr_ids(char *name)
{
    // 打印进程信息,包括进程名称,进程ID,父进程ID,进程组ID和终端进程组ID
    printf("%s: pid = %ld, ppid = %ld, pgrp = %ld, tpgrp = %ld\n",
           name,
           (long)getpid(),
           (long)getppid(),
           (long)getpgrp(),
           (long)tcgetpgrp(STDIN_FILENO));
    // 刷新标准输出缓冲区,确保信息立即输出
    fflush(stdout);
}

int main(void)
{
    char  c;    // 定义一个字符变量c
    pid_t pid;  // 定义一个进程ID变量pid

    pr_ids("parent");        // 打印当前进程ID和父进程ID,标识为"parent"
    if ((pid = fork()) < 0)  // 创建子进程
    {
        err_sys("fork error");  // 如果fork失败,打印错误信息
    }
    else if (pid > 0)
    { /* parent */  // 如果pid大于0,表示当前代码由父进程执行
        sleep(5);   // 父进程休眠5秒,以等待子进程执行
    }
    else
    { /* child */                                                 // 如果pid等于0,表示当前代码由子进程执行
        pr_ids("child");                                          // 打印当前进程ID和父进程ID,标识为"child"
        signal(SIGHUP, sig_hup);                                  // 设置SIGHUP信号的处理函数为sig_hup
        kill(getpid(), SIGTSTP);                                  // 发送SIGTSTP信号给自己,使子进程暂停
        pr_ids("child");                                          // 如果子进程被继续执行,再次打印进程ID
        if (read(STDIN_FILENO, &c, 1) != 1)                       // 尝试从控制终端读取一个字符
            printf("read error %d on controlling TTY\n", errno);  // 如果读取失败,打印错误信息
    }
    exit(0);  // 退出进程
}
