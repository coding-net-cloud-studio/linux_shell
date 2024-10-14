#include "apue.h"

// 根据不同的操作系统定义不同的ps命令
#ifdef SOLARIS
// 如果是SOLARIS系统,使用以下ps命令格式
#define PSCMD "ps -a -o pid,ppid,s,tty,comm"
#else
// 对于其他系统,使用以下ps命令格式
#define PSCMD "ps -o pid,ppid,state,tty,command"
#endif

/**
 * @brief 主函数,程序的入口点
 *
 * @return int 程序的退出状态码
 */
int main(void)
{
    pid_t pid;

    // 使用 fork() 函数创建一个新的进程
    if ((pid = fork()) < 0)
        // 如果 fork() 函数返回值小于 0,表示创建进程失败,调用 err_sys() 函数输出错误信息并终止程序
        err_sys("fork error");
    else if (pid == 0) /* child */
        // 如果 fork() 函数返回值等于 0,表示当前进程是子进程,调用 exit() 函数退出子进程
        exit(0);

    /* parent */
    // 父进程等待 4 秒,以便子进程有时间退出
    sleep(4);
    // 调用 system() 函数执行 PSCMD 命令,该命令用于列出当前系统中的进程信息
    system(PSCMD);

    // 主函数正常结束,返回 0
    exit(0);
}
