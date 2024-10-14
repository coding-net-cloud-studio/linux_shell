#include "apue.h"     // 引入apue库,提供系统调用和C标准库函数的封装
#include <termios.h>  // 引入termios库,提供终端I/O控制的功能

// 主函数
int main(void)
{
    struct termios term;  // 定义一个termios结构体变量,用于存储终端属性

    // 获取标准输入流的当前终端属性
    if (tcgetattr(STDIN_FILENO, &term) < 0)
        err_sys("tcgetattr error");  // 如果获取失败,打印错误信息并退出

    // 检查字符大小位掩码,并打印当前设置的位数
    // 以下代码根据term.c_cflag与CSIZE的位掩码结果,打印出每字节的位数.
    // term.c_cflag & CSIZE的结果决定了以下case语句的执行.
    switch (term.c_cflag & CSIZE)
    {
    // 如果结果是CS5,打印出"5 bits/byte"
    case CS5:
        printf("5 bits/byte\n");
        break;
    // 如果结果是CS6,打印出"6 bits/byte"
    case CS6:
        printf("6 bits/byte\n");
        break;
    // 如果结果是CS7,打印出"7 bits/byte"
    case CS7:
        printf("7 bits/byte\n");
        break;
    // 如果结果是CS8,打印出"8 bits/byte"
    case CS8:
        printf("8 bits/byte\n");
        break;
    // 如果结果不是上述任何一种情况,打印出"unknown bits/byte"
    default:
        printf("unknown bits/byte\n");
    }

    // 设置字符大小为8位
    term.c_cflag &= ~CSIZE;  // 清除字符大小位掩码
    term.c_cflag |= CS8;     // 设置字符大小位掩码为8位

    // 设置标准输入流的新终端属性
    if (tcsetattr(STDIN_FILENO, TCSANOW, &term) < 0)
        err_sys("tcsetattr error");  // 如果设置失败,打印错误信息并退出

    exit(0);  // 正常退出程序
}
