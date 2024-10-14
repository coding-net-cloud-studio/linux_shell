#include "apue.h"

#define BUFFSIZE 1024

// 信号处理函数,用于处理 SIGTSTP 信号
static void
sig_tstp(int signo) /* signal handler for SIGTSTP */
{
    sigset_t mask;

    // ... 移动光标到左下角,重置终端模式 ...

    /*
     * 在处理信号期间,SIGTSTP 会被阻塞,现在需要解除阻塞.
     */
    sigemptyset(&mask);                     // 清空信号集
    sigaddset(&mask, SIGTSTP);              // 将 SIGTSTP 添加到信号集中
    sigprocmask(SIG_UNBLOCK, &mask, NULL);  // 解除 SIGTSTP 的阻塞

    signal(SIGTSTP, SIG_DFL);  // 重置 SIGTSTP 的处理方式为默认处理方式

    kill(getpid(), SIGTSTP);  // 向自身发送 SIGTSTP 信号

    // 我们不会从 kill 返回,直到我们被继续执行

    signal(SIGTSTP, sig_tstp);  // 重新建立 SIGTSTP 的信号处理函数

    // ... 重置终端模式,重绘屏幕 ...
}
int main(void)
{
    int  n;              // 定义整型变量n,用于存储读取的字节数
    char buf[BUFFSIZE];  // 定义字符数组buf,用于存储从标准输入读取的数据

    /*
     * Only catch SIGTSTP if we're running with a job-control shell.
     * 如果我们是在支持作业控制的shell下运行,只捕获SIGTST执行暂停信号.
     */
    if (signal(SIGTSTP, SIG_IGN) == SIG_DFL)
        signal(SIGTSTP, sig_tstp);  // 如果SIGTSTP信号的处理是默认的,则将其处理函数设置为sig_tstp

    while ((n = read(STDIN_FILENO, buf, BUFFSIZE)) > 0)  // 从标准输入读取数据到buf,最多读取BUFFSIZE个字节
        if (write(STDOUT_FILENO, buf, n) != n)           // 将buf中的数据写入标准输出
            err_sys("write error");                      // 如果写入的字节数不等于n,表示写入出错,调用err_sys函数报告错误

    if (n < 0)                  // 如果读取的字节数小于0,表示读取出错
        err_sys("read error");  // 调用err_sys函数报告错误

    exit(0);  // 正常退出程序
}
