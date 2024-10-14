#include "apue.h"
#include <setjmp.h>
// 声明一个函数sig_alrm,它将处理SIGALRM信号
static void sig_alrm(int);
// 声明一个全局跳转缓冲区env_alrm,用于从sig_alrm函数跳回主程序
static jmp_buf env_alrm;

int main(void)
{
    int  n;
    char line[MAXLINE];

    // 注册SIGALRM信号的处理函数为sig_alrm
    if (signal(SIGALRM, sig_alrm) == SIG_ERR)
        // 如果注册失败,调用err_sys函数输出错误信息
        err_sys("signal(SIGALRM) error");
    // 设置跳转点,如果setjmp返回值不为0,则表示从sig_alrm函数跳转回来
    if (setjmp(env_alrm) != 0)
        // 调用err_quit函数输出超时错误信息并退出程序
        err_quit("read timeout");

    // 设置一个10秒的闹钟
    alarm(10);
    // 从标准输入读取数据到line数组中,最多读取MAXLINE个字符
    if ((n = read(STDIN_FILENO, line, MAXLINE)) < 0)
        // 如果读取失败,调用err_sys函数输出错误信息
        err_sys("read error");
    // 取消闹钟
    alarm(0);

    // 将读取到的字符写入到标准输出
    write(STDOUT_FILENO, line, n);
    // 程序正常结束
    exit(0);
}

// 定义SIGALRM信号的处理函数
static void
sig_alrm(int signo)
{
    // 通过longjmp函数跳回主程序中setjmp的位置,并设置返回值为1
    longjmp(env_alrm, 1);
}
