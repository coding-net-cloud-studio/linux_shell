#include "apue.h"

// 定义信号处理函数,用于处理SIGALRM信号
static void sig_alrm(int signo);

int main(void)
{
    int  n;              // 用于存储读取的字节数
    char line[MAXLINE];  // 定义一个字符数组用于存储从标准输入读取的数据

    // 设置SIGALRM信号的处理函数为sig_alrm
    // 为SIGALRM信号设置处理函数sig_alrm,如果设置失败,调用err_sys函数报告错误.
    // signal函数用于设置某个信号的处理方式,SIGALRM是一个定时器信号.
    // sig_alrm是处理SIGALRM信号的函数.
    // 如果signal函数返回SIG_ERR,表示设置信号处理函数失败.
    if (signal(SIGALRM, sig_alrm) == SIG_ERR)
        err_sys("signal(SIGALRM) error");

    // 设置一个10秒的闹钟,如果10秒内没有读取到数据,将触发SIGALRM信号
    alarm(10);
    // 从标准输入读取数据到line数组中
    // 从标准输入读取数据到line缓冲区,最多读取MAXLINE个字符
    // 如果读取的字节数小于0,则表示发生了错误,调用err_sys函数报告错误
    if ((n = read(STDIN_FILENO, line, MAXLINE)) < 0)
        err_sys("read error");

    // 读取完成后取消闹钟
    alarm(0);

    // 将读取到的数据写入标准输出
    write(STDOUT_FILENO, line, n);
    // 正常退出程序
    exit(0);
}

// SIGALRM信号的处理函数
static void
sig_alrm(int signo)
{
    /* 什么都不做,只是返回以中断read */
}
