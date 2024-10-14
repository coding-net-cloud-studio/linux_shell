#include "apue.h"
#include <sys/wait.h>

// 初始化环境变量数组
char *env_init[] = {"USER=unknown", "PATH=/tmp", NULL};

int main(void)
{
    pid_t pid;

    // 第一次fork操作
    if ((pid = fork()) < 0)
    {
        // 如果fork失败,打印错误信息并退出
        err_sys("fork error");
    }
    else if (pid == 0)
    { /* 子进程 */
        // 使用execle函数执行echoall程序,指定环境变量
        // execle函数用于替换当前进程的镜像,执行一个新的程序.
        // 参数包括要执行的程序文件的路径,传递给新程序的参数列表,以及新程序的环境变量数组.
        // 如果execle函数成功执行,当前进程将被新程序替换,不会返回.
        // 如果失败,则返回-1,并设置errno以指示错误.
        if (execle("/home/sar/bin/echoall", "echoall", "myarg1", "MY ARG2", (char *)0, env_init) < 0)

            // 如果execle失败,打印错误信息并退出
            err_sys("execle error");
    }

    // 等待子进程结束
    if (waitpid(pid, NULL, 0) < 0)
        // 如果waitpid失败,打印错误信息并退出
        err_sys("wait error");

    // 第二次fork操作
    if ((pid = fork()) < 0)
    {
        // 如果fork失败,打印错误信息并退出
        err_sys("fork error");
    }
    else if (pid == 0)
    { /* 子进程 */
        // 使用execlp函数执行echoall程序,继承父进程的环境变量
        if (execlp("echoall", "echoall", "only 1 arg", (char *)0) < 0)
            // 如果execlp失败,打印错误信息并退出
            err_sys("execlp error");
    }

    // 父进程退出
    exit(0);
}
