#include "apue.h"
#include <sys/wait.h>

int main(void)
{
    char  buf[MAXLINE]; /* from apue.h 定义一个字符数组用于存储用户输入的命令 */
    pid_t pid;          /* 定义一个进程ID变量 */
    int   status;       /* 定义一个整型变量用于存储子进程的退出状态 */

    printf("%% "); /* print prompt (printf requires %% to print %) 打印命令提示符 */

    /* 循环读取用户输入的命令 */
    while (fgets(buf, MAXLINE, stdin) != NULL)
    {
        if (buf[strlen(buf) - 1] == '\n')
            buf[strlen(buf) - 1] = 0; /* replace newline with null 去除输入命令末尾的换行符 */

        if ((pid = fork()) < 0)
        {
            err_sys("fork error"); /* 如果fork失败,打印错误信息并退出 */
        }
        else if (pid == 0)
        {                                         /* child 子进程执行的代码块 */
            execlp(buf, buf, (char *)0);          /* 使用execlp函数执行用户输入的命令 */
            err_ret("couldn't execute: %s", buf); /* 如果execlp执行失败,打印错误信息 */
            exit(127);                            /* 退出子进程 */
        }

        /* parent 父进程执行的代码块 */
        if ((pid = waitpid(pid, &status, 0)) < 0)
            err_sys("waitpid error"); /* 如果waitpid等待子进程失败,打印错误信息 */
        printf("%% ");                /* 打印命令提示符,等待下一次用户输入 */
    }
    exit(0); /* 程序正常退出 */
}
