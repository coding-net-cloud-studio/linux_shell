#include "apue.h"      // 引入APUE库,提供系统编程接口
#include <sys/wait.h>  // 引入wait系统调用头文件,用于等待子进程结束

// 以下代码段应包含具体的函数实现和注释

// 信号捕获函数声明
static void sig_int(int);

int main(void)
{
    char  buf[MAXLINE];  // 从apue.h中定义的缓冲区
    pid_t pid;           // 进程ID
    int   status;        // 进程状态

    // 设置SIGINT信号的处理函数为sig_int
    // 为SIGINT信号(通常是Ctrl+C)注册一个处理函数sig_int
    // 如果注册失败,调用err_sys函数并传递错误信息"signal error"
    if (signal(SIGINT, sig_int) == SIG_ERR)
        err_sys("signal error");

    printf("%% ");  // 打印提示符,printf需要用%%来打印%

    // 从标准输入读取数据直到EOF
    while (fgets(buf, MAXLINE, stdin) != NULL)
    {
        // 去除输入行末尾的换行符
        if (buf[strlen(buf) - 1] == '\n')
            buf[strlen(buf) - 1] = 0;

        // 创建子进程
        if ((pid = fork()) < 0)
        {
            // fork失败
            // 如果fork系统调用失败,返回-1,这里调用err_sys函数打印错误信息并退出程序
            err_sys("fork error");
        }
        else if (pid == 0)
        {  // 子进程
            // 使用execlp执行用户输入的命令
            // execlp函数用于在当前进程的上下文中执行一个新的程序,第一个参数是程序名,后续参数是传递给新程序的参数
            execlp(buf, buf, (char *)0);
            // 如果execlp失败,打印错误信息并退出
            // 如果execlp函数执行失败,不会返回,除非发生了错误,此时会返回-1,并调用err_ret函数打印错误信息
            err_ret("couldn't execute: %s", buf);
            exit(127);  // 退出子进程,127通常表示无法执行的命令
        }

        // 父进程
        // 等待子进程结束
        if ((pid = waitpid(pid, &status, 0)) < 0)
            err_sys("waitpid error");
        printf("%% ");  // 打印提示符
    }
    exit(0);  // 正常退出
}

// SIGINT信号处理函数
void sig_int(int signo)
{
    printf("interrupt\n%% ");  // 打印中断信息并重新打印提示符
}
