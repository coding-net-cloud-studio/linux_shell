#include "apue.h"      // 引入apue库,提供系统编程接口
#include <sys/wait.h>  // 引入sys/wait.h头文件,提供进程等待相关函数

int main(void)
{
    pid_t pid;

    // 创建第一个子进程
    if ((pid = fork()) < 0)
    {
        err_sys("fork error");  // 如果fork失败,打印错误信息
    }
    else if (pid == 0)
    { /* first child */
        // 第一个子进程再次创建子进程
        if ((pid = fork()) < 0)
            err_sys("fork error");  // 如果fork失败,打印错误信息
        else if (pid > 0)
            exit(0); /* parent from second fork == first child */  // 第一个子进程退出,因为它已经是第二个子进程的父进程

        /*
         * 我们是第二个子进程;一旦我们的真正父进程在上方的exit()调用中退出,
         * 我们的父进程就变成了init.在这里,我们将继续执行,知道当我们完成时,
         * init将会回收我们的状态.
         */
        sleep(2);                                                     // 第二个子进程休眠2秒
        printf("second child, parent pid = %ld\n", (long)getppid());  // 打印第二个子进程的父进程ID
        exit(0);                                                      // 第二个子进程退出
    }

    // 等待第一个子进程结束
    if (waitpid(pid, NULL, 0) != pid) /* wait for first child */
        err_sys("waitpid error");     // 如果waitpid失败,打印错误信息

    /*
     * 我们是父进程(原始进程);我们继续执行,知道我们不是第二个子进程的父进程.
     */
    exit(0);  // 父进程退出
}
