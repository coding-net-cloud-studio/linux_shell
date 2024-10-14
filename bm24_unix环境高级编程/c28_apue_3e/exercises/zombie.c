#include "apue.h"

// 根据不同的操作系统定义不同的ps命令
#ifdef SOLARIS
#define PSCMD "ps -a -o pid,ppid,s,tty,comm"  // Solaris系统的ps命令格式
#else
#define PSCMD "ps -o pid,ppid,state,tty,command"  // 其他系统的ps命令格式
#endif

int main(void)
{
    pid_t pid;  // 定义进程ID变量

    // 使用fork函数创建子进程
    if ((pid = fork()) < 0)
        err_sys("fork error");  // 如果fork失败,打印错误信息并退出
    else if (pid == 0)          /* child */
        exit(0);                // 如果是子进程,直接退出

    /* parent */
    sleep(4);       // 父进程等待4秒,以确保子进程已经退出
    system(PSCMD);  // 执行ps命令,显示当前系统进程状态

    exit(0);  // 父进程退出
}
