// 引入apue库的头文件,该库提供了一套用于编写可移植UNIX应用程序的函数.
#include "apue.h"

// 引入errno头文件,该文件定义了一个全局变量errno,用于报告系统调用的错误.
#include <errno.h>

// sig_hup函数用于处理SIGHUP信号
// 当进程收到SIGHUP信号时,此函数会被调用
// signo参数表示接收到的信号编号
static void
sig_hup(int signo)
{
    // 打印接收到的信号信息以及当前进程的PID
    printf("SIGHUP received, pid = %ld\n", (long)getpid());
}

// pr_ids函数用于打印进程相关的ID信息
// name参数表示进程的名称
static void
pr_ids(char *name)
{
    // 打印进程名称,PID,父进程ID,进程组ID以及前台进程组ID
    // 打印当前进程及其父进程的信息,包括进程ID,父进程ID,进程组ID和前台进程组ID
    printf("%s: pid = %ld, ppid = %ld, pgrp = %ld, tpgrp = %ld\n",
           // name: 进程名称
           name,
           // getpid(): 获取当前进程的ID
           (long)getpid(),
           // getppid(): 获取当前进程的父进程ID
           (long)getppid(),
           // getpgrp(): 获取当前进程的进程组ID
           (long)getpgrp(),
           // tcgetpgrp(STDIN_FILENO): 获取与标准输入关联的前台进程组ID
           (long)tcgetpgrp(STDIN_FILENO));

    // 刷新标准输出缓冲区,确保信息立即显示
    fflush(stdout);
}
int main(void)
{
    char  c;    // 定义一个字符变量c
    pid_t pid;  // 定义一个进程ID变量pid

    pr_ids("parent");        // 打印当前进程的ID和PPID,标识为"parent"
    if ((pid = fork()) < 0)  // 创建子进程
    {
        err_sys("fork error");  // 如果fork失败,打印错误信息并退出
    }
    else if (pid > 0)  // 父进程
    {
        sleep(5);  // 父进程休眠5秒,以等待子进程先停止自己
    }
    else  // 子进程
    {
        pr_ids("child");                                          // 打印当前进程的ID和PPID,标识为"child"
        signal(SIGHUP, sig_hup);                                  // 设置SIGHUP信号的处理函数为sig_hup
        kill(getpid(), SIGTSTP);                                  // 发送SIGTSTP信号给自己,使子进程停止
        pr_ids("child");                                          // 如果子进程被继续执行,再次打印进程ID和PPID
        if (read(STDIN_FILENO, &c, 1) != 1)                       // 尝试从控制终端读取一个字符
            printf("read error %d on controlling TTY\n", errno);  // 如果读取失败,打印错误信息
    }
    exit(0);  // 退出进程
}
