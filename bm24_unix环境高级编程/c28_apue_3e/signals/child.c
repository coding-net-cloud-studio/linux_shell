#include "apue.h"      // 引入apue库,提供系统编程接口
#include <sys/wait.h>  // 引入wait系统调用头文件,用于等待子进程结束

// 定义SIGCLD信号的处理函数
static void sig_cld(int);

int main()
{
    pid_t pid;  // 定义进程ID变量

    // 设置SIGCLD信号的处理函数为sig_cld
    if (signal(SIGCLD, sig_cld) == SIG_ERR)
        perror("signal error");  // 如果设置失败,输出错误信息

    // 创建子进程
    if ((pid = fork()) < 0)
    {
        perror("fork error");  // 如果创建失败,输出错误信息
    }
    else if (pid == 0)
    {              /* child */
        sleep(2);  // 子进程休眠2秒
        _exit(0);  // 子进程退出
    }

    pause(); /* parent */  // 父进程暂停,等待信号
    exit(0);
}

// SIGCLD信号的处理函数
static void
sig_cld(int signo) /* interrupts pause() */
{

    // 声明进程ID变量pid,用于存储fork()函数返回的子进程ID
    pid_t pid;

    // 声明整型变量status,用于存储wait()或waitpid()函数的返回状态
    int status;

    printf("SIGCLD received\n");  // 收到SIGCLD信号时的提示信息

    // 重新设置SIGCLD信号的处理函数为sig_cld
    if (signal(SIGCLD, sig_cld) == SIG_ERR)
        perror("signal error");  // 如果设置失败,输出错误信息

    // 等待子进程结束,并获取其退出状态
    if ((pid = wait(&status)) < 0)
        perror("wait error");  // 如果等待失败,输出错误信息

    printf("pid = %d\n", pid);  // 输出结束的子进程的PID
}
