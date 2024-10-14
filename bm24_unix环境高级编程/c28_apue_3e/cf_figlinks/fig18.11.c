#include "apue.h"     // 引入apue库,提供系统调用和C标准库函数的封装
#include <termios.h>  // 引入termios.h头文件,用于控制终端I/O

// 以下代码段没有具体的函数方法,因此无需添加标准doc注释.
// 如果代码段中有具体的函数实现,应当按照函数说明的标准格式添加注释.

int main(void)
{
    struct termios term;  // 定义一个termios结构体变量,用于存储终端属性

    // 获取标准输入流的当前终端属性

    // 获取标准输入的终端属性
    // 如果获取失败,调用err_sys函数打印错误信息并退出程序
    if (tcgetattr(STDIN_FILENO, &term) < 0)
        err_sys("tcgetattr error");  // 如果获取失败,打印错误信息并退出

    // 检查当前设置的每字节位数
    switch (term.c_cflag & CSIZE)
    {
    case CS5:
        printf("5 bits/byte\n");  // 如果是5位/字节,打印信息
        break;
    case CS6:
        printf("6 bits/byte\n");  // 如果是6位/字节,打印信息
        break;
    case CS7:
        printf("7 bits/byte\n");  // 如果是7位/字节,打印信息
        break;
    case CS8:
        printf("8 bits/byte\n");  // 如果是8位/字节,打印信息
        break;
    default:
        printf("unknown bits/byte\n");  // 如果不是上述情况,打印未知信息
    }

    term.c_cflag &= ~CSIZE;  // 清除c_cflag中的CSIZE位,以便设置新的位数
    term.c_cflag |= CS8;     // 设置c_cflag中的CSIZE位为CS8,即每字节8位

    // 设置标准输入流的终端属性为新的设置
    if (tcsetattr(STDIN_FILENO, TCSANOW, &term) < 0)
        err_sys("tcsetattr error");  // 如果设置失败,打印错误信息并退出

    exit(0);  // 正常退出程序
}
