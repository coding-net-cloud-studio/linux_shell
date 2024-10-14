#include "apue.h"
#include <sys/wait.h>

int main(void)
{
    char  buf[MAXLINE]; /* from apue.h */
    pid_t pid;          // 进程ID
    int   status;       // 子进程退出状态

    printf("%% "); /* print prompt (printf requires %% to print %) */
    // 循环读取用户输入的命令
    while (fgets(buf, MAXLINE, stdin) != NULL)
    {
        // 检查缓冲区最后一个字符是否为换行符
        if (buf[strlen(buf) - 1] == '\n')
            // 如果是换行符,则将其替换为空字符(null)
            buf[strlen(buf) - 1] = 0; /* replace newline with null */

        if ((pid = fork()) < 0)
        {
            err_sys("fork error");  // 如果fork失败,打印错误信息并退出
        }
        else if (pid == 0)
        {                                          /* child */
            execlp(buf, buf, (char *)0);           // 子进程执行用户输入的命令
            err_ret("couldn't execute: %s", buf);  // 如果execlp失败,打印错误信息
            exit(127);                             // 退出子进程
        }

        /* parent */
        if ((pid = waitpid(pid, &status, 0)) < 0)  // 父进程等待子进程结束
            err_sys("waitpid error");              // 如果waitpid失败,打印错误信息
        printf("%% ");                             // 打印提示符,准备接收下一个命令
    }
    exit(0);  // 退出主程序
}
