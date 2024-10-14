#include "apue.h"

int main(void)
{
    pid_t pid;

    // 第一次fork,创建子进程
    if ((pid = fork()) < 0)
        err_sys("fork error");  // 如果fork失败,打印错误并退出
    else if (pid != 0)
    {                                                /* parent */
        sleep(2);                                    // 父进程等待2秒
        exit(2); /* terminate with exit status 2 */  // 父进程退出,状态码为2
    }

    // 第二次fork,创建第一个子进程
    if ((pid = fork()) < 0)
        err_sys("fork error");  // 如果fork失败,打印错误并退出
    else if (pid != 0)
    {                                            /* first child */
        sleep(4);                                // 第一个子进程等待4秒
        abort(); /* terminate with core dump */  // 第一个子进程异常退出,生成核心转储
    }

    // 第三次fork,创建第二个子进程
    if ((pid = fork()) < 0)
        err_sys("fork error");  // 如果fork失败,打印错误并退出
    else if (pid != 0)
    {                                                                    /* second child */
        execl("/bin/dd", "dd", "if=/etc/passwd", "of=/dev/null", NULL);  // 第二个子进程执行dd命令
        exit(7); /* shouldn't get here */                                // 如果execl失败,不应该执行到这里,退出状态码为7
    }

    // 第四次fork,创建第三个子进程
    if ((pid = fork()) < 0)
        err_sys("fork error");  // 如果fork失败,打印错误并退出
    else if (pid != 0)
    {                               /* third child */
        sleep(8);                   // 第三个子进程等待8秒
        exit(0); /* normal exit */  // 第三个子进程正常退出,状态码为0
    }

    sleep(6);                          // 第四个子进程等待6秒
    kill(getpid(), SIGKILL);           // 第四个子进程发送SIGKILL信号给自己,强制终止
    exit(6); /* shouldn't get here */  // 如果kill之前程序没有终止,不应该执行到这里,退出状态码为6
}
