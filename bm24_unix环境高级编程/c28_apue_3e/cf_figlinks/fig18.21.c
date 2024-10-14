#include "apue.h"

// <summary>
// 信号捕获处理函数
// </summary>
// <param name="signo">捕获到的信号编号</param>
static void
sig_catch(int signo)
{
    // 打印信号捕获信息
    printf("signal caught\n");
    // 重置标准输入设备(TTY)的状态
    tty_reset(STDIN_FILENO);
    // 退出程序
    exit(0);
}

int main(void)
{
    int  i;  // 定义整型变量i,用于存储读取的字符数
    char c;  // 定义字符变量c,用于存储从标准输入读取的字符

    // 捕获SIGINT信号,如果出错则调用err_sys函数报告错误
    if (signal(SIGINT, sig_catch) == SIG_ERR)
        err_sys("signal(SIGINT) error");
    // 捕获SIGQUIT信号,如果出错则调用err_sys函数报告错误
    if (signal(SIGQUIT, sig_catch) == SIG_ERR)
        err_sys("signal(SIGQUIT) error");
    // 捕获SIGTERM信号,如果出错则调用err_sys函数报告错误
    if (signal(SIGTERM, sig_catch) == SIG_ERR)
        err_sys("signal(SIGTERM) error");

    // 将标准输入设置为原始模式,如果出错则调用err_sys函数报告错误
    if (tty_raw(STDIN_FILENO) < 0)
        err_sys("tty_raw error");
    printf("Enter raw mode characters, terminate with DELETE\n");
    // 循环读取标准输入的单个字符,直到读取到ASCII码为DELETE的字符
    while ((i = read(STDIN_FILENO, &c, 1)) == 1)
    {
        if ((c &= 255) == 0177) /* 0177 = ASCII DELETE */
            break;
        printf("%o\n", c);  // 打印读取到的字符的八进制表示
    }
    // 将标准输入重置为默认模式,如果出错则调用err_sys函数报告错误
    if (tty_reset(STDIN_FILENO) < 0)
        err_sys("tty_reset error");
    if (i <= 0)
        err_sys("read error");  // 如果读取错误或到达文件末尾,报告错误

    // 将标准输入设置为cbreak模式,如果出错则调用err_sys函数报告错误
    // 如果 tty_cbreak(STDIN_FILENO) 返回值小于 0,则表示调用失败
    // tty_cbreak() 函数用于将终端设置为原始模式,禁用行缓冲
    // STDIN_FILENO 是标准输入的文件描述符
    if (tty_cbreak(STDIN_FILENO) < 0)
        // err_sys() 函数用于打印系统错误信息并退出程序
        // "tty_cbreak error" 是自定义的错误信息
        err_sys("tty_cbreak error");

    printf("\nEnter cbreak mode characters, terminate with SIGINT\n");
    // 循环读取标准输入的单个字符,直到接收到SIGINT信号

    // 从标准输入读取一个字符
    while ((i = read(STDIN_FILENO, &c, 1)) == 1)
    {
        // 将字符的ASCII码值限制在0-255范围内
        c &= 255;
        // 打印读取到的字符的八进制表示
        printf("%o\n", c);
    }

    // 将标准输入重置为默认模式,如果出错则调用err_sys函数报告错误
    // 如果 tty_reset 函数返回值小于 0,表示重置终端失败
    if (tty_reset(STDIN_FILENO) < 0)
        // 输出错误信息并退出程序
        err_sys("tty_reset error");

    if (i <= 0)
        err_sys("read error");  // 如果读取错误或到达文件末尾,报告错误

    exit(0);  // 正常退出程序
}
