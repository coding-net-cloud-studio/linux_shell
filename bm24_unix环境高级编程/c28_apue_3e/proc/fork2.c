#include "apue.h"
#include <sys/wait.h>

int main(void)
{
    pid_t pid;

    // 创建第一个子进程
    if ((pid = fork()) < 0)
    {
        err_sys("fork error");  // 如果fork失败,打印错误信息并退出
    }
    else if (pid == 0)
    { /* first child */
        // 第一个子进程再次fork,创建第二个子进程
        if ((pid = fork()) < 0)
            err_sys("fork error");  // 如果fork失败,打印错误信息并退出
        else if (pid > 0)
            exit(0);  // 第一个子进程退出,因为它已经是第二个子进程的父进程了

        /*
         * 我们是第二个子进程;一旦我们的亲生父进程在上方的exit()调用后退出,
         * 我们的父进程就变成了init.在这里,我们将继续执行,知道当我们完成时,
         * init将会回收我们的状态.
         */
        sleep(2);                                                     // 第二个子进程休眠2秒
        printf("second child, parent pid = %ld\n", (long)getppid());  // 打印第二个子进程的父进程ID
        exit(0);                                                      // 第二个子进程退出
    }

    // 等待第一个子进程结束
    if (waitpid(pid, NULL, 0) != pid)
        err_sys("waitpid error");  // 如果waitpid失败,打印错误信息并退出

    /*
     * 我们是父进程(原始进程);我们继续执行,知道我们不是第二个子进程的父进程.
     */
    exit(0);  // 父进程退出
}
