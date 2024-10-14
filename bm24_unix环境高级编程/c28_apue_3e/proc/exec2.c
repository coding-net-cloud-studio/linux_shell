#include "apue.h"
#include <sys/wait.h>

int main(void)
{
    pid_t pid;  // 定义一个进程ID变量

    // 使用fork函数创建子进程
    if ((pid = fork()) < 0)
    {
        err_sys("fork error");  // 如果fork失败,打印错误信息并退出
    }
    else if (pid == 0)
    { /* child */
        // 子进程执行execl函数,替换当前进程的镜像为指定的程序
        // 第一个参数是要执行的程序路径,后面的参数是传递给该程序的命令行参数
        if (execl("/home/sar/bin/testinterp",
                  "testinterp",
                  "myarg1",
                  "MY ARG2",
                  (char *)0) < 0)
            err_sys("execl error");  // 如果execl失败,打印错误信息并退出
    }
    // 父进程等待子进程结束
    if (waitpid(pid, NULL, 0) < 0) /* parent */
        err_sys("waitpid error");  // 如果waitpid失败,打印错误信息并退出
    exit(0);                       // 程序正常退出
}
