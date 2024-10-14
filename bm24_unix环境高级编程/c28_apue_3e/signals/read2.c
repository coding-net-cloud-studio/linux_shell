#include "apue.h"    // 引入APUE库的头文件
#include <setjmp.h>  // 引入setjmp.h头文件,用于非局部跳转

// 定义一个静态的jmp_buf类型的变量,用于存储longjmp的跳转环境
static jmp_buf env_alrm;
static void    sig_alrm(int);

// 定义信号处理函数,当接收到SIGALRM信号时调用
static void sig_alrm(int signo)
{
    longjmp(env_alrm, 1);  // 使用longjmp进行非局部跳转,跳转到env_alrm定义的位置,并返回值1
}

// 主函数
int main(void)
{
    int  n;              // 用于存储读取的字节数
    char line[MAXLINE];  // 用于存储从标准输入读取的行

    // 设置SIGALRM信号的处理函数为sig_alrm
    if (signal(SIGALRM, sig_alrm) == SIG_ERR)
        err_sys("signal(SIGALRM) error");

    // 设置跳转点,用于非局部跳转
    if (setjmp(env_alrm) != 0)
        err_quit("read timeout");

    // 设置闹钟信号,10秒后发送SIGALRM信号
    alarm(10);

    // 从标准输入读取数据到line,最多读取MAXLINE-1个字节
    if ((n = read(STDIN_FILENO, line, MAXLINE)) < 0)
        err_sys("read error");

    // 读取完成后取消闹钟信号
    alarm(0);

    // 将读取到的数据写入标准输出
    write(STDOUT_FILENO, line, n);

    // 正常退出程序
    exit(0);
}
