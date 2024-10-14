#include "apue.h"

// 信号处理函数,捕获信号并重置终端,然后退出程序
static void
sig_catch(int signo)
{
    printf("signal caught\n");  // 打印信号捕获信息
    tty_reset(STDIN_FILENO);    // 重置终端
    exit(0);                    // 退出程序
}

int main(void)
{
    // 声明一个整型变量 i,用于存储整数值
    int i;

    // 声明一个字符型变量 c,用于存储单个字符
    char c;

    // 捕获SIGINT信号,当用户按下Ctrl+C时触发
    if (signal(SIGINT, sig_catch) == SIG_ERR)
        err_sys("signal(SIGINT) error");
    // 捕获SIGQUIT信号,当用户按下Ctrl+\时触发
    if (signal(SIGQUIT, sig_catch) == SIG_ERR)
        err_sys("signal(SIGQUIT) error");
    // 捕获SIGTERM信号,通常由kill命令发送
    if (signal(SIGTERM, sig_catch) == SIG_ERR)
        err_sys("signal(SIGTERM) error");

    // 将终端设置为原始模式,以便读取单个字符
    if (tty_raw(STDIN_FILENO) < 0)
        err_sys("tty_raw error");
    printf("Enter raw mode characters, terminate with DELETE\n");
    // 循环读取字符,直到读取到ASCII码为DELETE的字符
    while ((i = read(STDIN_FILENO, &c, 1)) == 1)
    {
        if ((c &= 255) == 0177)  // 0177是ASCII码中的DELETE字符
            break;
        printf("%o\n", c);  // 打印读取到的字符的八进制表示
    }
    // 重置终端
    // 检查tty_reset函数调用是否成功,如果不成功则输出错误信息
    if (tty_reset(STDIN_FILENO) < 0)
        err_sys("tty_reset error");

    // 检查读取操作的结果,如果i小于等于0,则表示读取错误,输出错误信息
    if (i <= 0)
        err_sys("read error");

    // 将终端设置为cbreak模式,允许特殊字符被传递给程序
    // 如果 tty_cbreak(STDIN_FILENO) 返回值小于 0,表示设置终端为 cbreak 模式失败,调用 err_sys 函数输出错误信息
    if (tty_cbreak(STDIN_FILENO) < 0)
        err_sys("tty_cbreak error");

    // 提示用户输入 cbreak 模式的字符,以 SIGINT 结束输入
    printf("\nEnter cbreak mode characters, terminate with SIGINT\n");

    // 循环读取字符,直到捕获到SIGINT信号
    while ((i = read(STDIN_FILENO, &c, 1)) == 1)
    {
        c &= 255;
        printf("%o\n", c);  // 打印读取到的字符的八进制表示
    }
    // 重置终端
    // 检查tty_reset函数调用是否成功,如果不成功则输出错误信息
    if (tty_reset(STDIN_FILENO) < 0)
        err_sys("tty_reset error");

    // 检查读取操作的结果i是否小于等于0,如果是则输出错误信息
    if (i <= 0)
        err_sys("read error");

    exit(0);  // 退出程序
}
