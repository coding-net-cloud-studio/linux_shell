#include "apue.h"      // 包含APUE库头文件
#include <sys/wait.h>  // 包含系统等待子进程的头文件

// 初始化环境变量数组
char *env_init[] = {"USER=unknown", "PATH=/tmp", NULL};

int main(void)
{
    pid_t pid;  // 定义进程ID变量

    // 第一次fork操作
    if ((pid = fork()) < 0)
    {
        err_sys("fork error");  // 如果fork失败,打印错误信息并退出
    }
    else if (pid == 0)
    {
        /* 子进程执行execle,指定路径和环境变量 */
        if (execle("/home/sar/bin/echoall", "echoall", "myarg1", "MY ARG2", (char *)0, env_init) < 0)
            err_sys("execle error");  // 如果execle执行失败,打印错误信息并退出
    }

    // 等待第一个子进程结束
    if (waitpid(pid, NULL, 0) < 0)
        err_sys("wait error");  // 如果等待失败,打印错误信息并退出

    // 第二次fork操作
    if ((pid = fork()) < 0)
    {
        err_sys("fork error");  // 如果fork失败,打印错误信息并退出
    }
    else if (pid == 0)
    {
        /* 子进程执行execlp,指定文件名并继承环境变量 */
        if (execlp("echoall", "echoall", "only 1 arg", (char *)0) < 0)
            err_sys("execlp error");  // 如果execlp执行失败,打印错误信息并退出
    }

    exit(0);  // 主进程退出
}
