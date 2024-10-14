#include "apue.h"       // 引入apue库,提供系统编程接口
#include <termios.h>    // 引入termios库,用于控制终端I/O
#ifndef TIOCGWINSZ      // 如果未定义TIOCGWINSZ宏
#include <sys/ioctl.h>  // 则引入ioctl库,用于获取终端窗口大小
#endif

static void
pr_winsize(int fd)  // 定义函数pr_winsize,参数为文件描述符fd
{
    struct winsize size;  // 定义winsize结构体变量size,用于存储窗口大小信息

    if (ioctl(fd, TIOCGWINSZ, (char *)&size) < 0)               // 使用ioctl函数获取终端窗口大小,如果出错则返回
        err_sys("TIOCGWINSZ error");                            // 如果ioctl调用失败,打印错误信息
    printf("%d rows, %d columns\n", size.ws_row, size.ws_col);  // 打印终端窗口的行数和列数
}

// fig18.22.c

// 当接收到 SIGWINCH 信号时调用的处理函数
static void
sig_winch(int signo)
{
    // 打印信号接收信息
    printf("SIGWINCH received\n");

    // 打印当前终端窗口的大小信息
    // STDIN_FILENO 是标准输入文件描述符,通常指向终端
    // pr_winsize 是一个假设存在的函数,用于打印窗口大小信息
    pr_winsize(STDIN_FILENO);
}

/**
 * 主函数,用于检测标准输入是否为终端,并在窗口大小改变时进行处理.
 */
int main(void)
{
    // 检查标准输入是否为终端,如果不是,则退出程序
    if (isatty(STDIN_FILENO) == 0)
        exit(1);

    // 设置窗口大小改变时的信号处理函数,如果设置失败,则输出错误信息并退出
    if (signal(SIGWINCH, sig_winch) == SIG_ERR)
        err_sys("signal error");

    // 打印初始窗口大小
    pr_winsize(STDIN_FILENO);

    // 进入无限循环,挂起进程等待信号
    for (;;)
        pause();
}
