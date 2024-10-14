#include "apue.h"
#include <termios.h>
#ifndef TIOCGWINSZ
#include <sys/ioctl.h>
#endif

// 函数功能:打印当前终端窗口的大小
// 参数:fd - 文件描述符,通常为标准输入(STDIN_FILENO)
static void
pr_winsize(int fd)
{
    struct winsize size;  // 定义一个winsize结构体来存储窗口大小

    // 使用ioctl系统调用获取终端窗口大小
    if (ioctl(fd, TIOCGWINSZ, (char *)&size) < 0)
        err_sys("TIOCGWINSZ error");                            // 如果出错,打印错误信息并退出
    printf("%d rows, %d columns\n", size.ws_row, size.ws_col);  // 打印行数和列数
}

// 函数功能:处理窗口大小改变信号(SIGWINCH)
// 参数:signo - 信号编号
static void
sig_winch(int signo)
{
    printf("SIGWINCH received\n");  // 打印信号接收信息
    pr_winsize(STDIN_FILENO);       // 调用pr_winsize函数打印新的窗口大小
}

int main(void)
{
    // 检查标准输入是否为一个终端,如果不是则退出程序
    if (isatty(STDIN_FILENO) == 0)
        exit(1);
    // 设置SIGWINCH信号的处理函数为sig_winch
    if (signal(SIGWINCH, sig_winch) == SIG_ERR)
        err_sys("signal error");  // 如果出错,打印错误信息并退出
    pr_winsize(STDIN_FILENO);     // 打印初始窗口大小
    for (;;)                      // 无限循环
        pause();                  // 暂停程序,等待信号
}
