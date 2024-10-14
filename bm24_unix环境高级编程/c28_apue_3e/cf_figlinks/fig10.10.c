#include "apue.h"

// 定义信号处理函数,用于处理SIGALRM信号
static void sig_alrm(int signo);

int main(void)
{
    int  n;              // 用于存储读取的字节数
    char line[MAXLINE];  // 定义一个字符数组用于存储从标准输入读取的数据

    // 注册SIGALRM信号的处理函数为sig_alrm
    if (signal(SIGALRM, sig_alrm) == SIG_ERR)
        err_sys("signal(SIGALRM) error");

    // 设置一个定时器,10秒后发送SIGALRM信号
    alarm(10);

    // 从标准输入读取数据到line数组中
    if ((n = read(STDIN_FILENO, line, MAXLINE)) < 0)
        err_sys("read error");

    // 取消定时器,防止SIGALRM信号再次触发
    alarm(0);

    // 将读取到的数据写入标准输出
    write(STDOUT_FILENO, line, n);

    // 正常退出程序
    exit(0);
}

// SIGALRM信号的处理函数
static void sig_alrm(int signo)
{
    // 不做任何处理,只是返回以中断read操作
}
