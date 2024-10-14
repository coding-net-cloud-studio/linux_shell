#include "apue.h"      // 引入APUE库的头文件
#include <sys/wait.h>  // 引入系统等待子进程状态的头文件

// 定义信号处理函数
static void sig_int(int signo);

int main(void)
{
    char  buf[MAXLINE];  // 定义缓冲区用于存储用户输入的命令
    pid_t pid;           // 定义进程ID变量
    int   status;        // 定义子进程状态的变量

    // 注册SIGINT信号的处理函数为sig_int
    if (signal(SIGINT, sig_int) == SIG_ERR)
        err_sys("signal error");

    printf("%% ");  // 打印命令提示符

    // 循环读取用户输入的命令
    while (fgets(buf, MAXLINE, stdin) != NULL)
    {
        if (buf[strlen(buf) - 1] == '\n')
            buf[strlen(buf) - 1] = 0;  // 将换行符替换为空字符

        // 创建子进程
        if ((pid = fork()) < 0)
        {
            err_sys("fork error");  // 如果fork失败,打印错误信息并退出
        }
        else if (pid == 0)
        {                                          // 子进程执行的代码块
            execlp(buf, buf, (char *)0);           // 执行用户输入的命令
            err_ret("couldn't execute: %s", buf);  // 如果execlp失败,打印错误信息
            exit(127);                             // 退出子进程
        }

        // 父进程执行的代码块
        if ((pid = waitpid(pid, &status, 0)) < 0)
            err_sys("waitpid error");  // 如果waitpid失败,打印错误信息
        printf("%% ");                 // 打印命令提示符
    }
    exit(0);  // 程序正常退出
}

// 定义信号处理函数
void sig_int(int signo)
{
    printf("interrupt\n%% ");  // 打印中断信息并重新打印命令提示符
}
