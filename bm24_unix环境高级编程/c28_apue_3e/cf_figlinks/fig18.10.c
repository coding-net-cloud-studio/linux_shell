#include "apue.h"     // 引入apue库,提供系统调用和实用函数的封装
#include <termios.h>  // 引入termios库,用于控制终端I/O

/*
 * main - 禁用终端中断字符(INTR)并将EOF设置为Control-B
 *
 * 返回: 成功时退出状态码为0,失败时调用err_quit或err_sys并退出
 */
int main(void)
{
    struct termios term;      // 用于存储终端属性的结构体
    long           vdisable;  // 用于存储_VDISABLE值

    // 检查标准输入是否为终端设备
    if (isatty(STDIN_FILENO) == 0)
        err_quit("standard input is not a terminal device");

    // 获取_VDISABLE值,该值用于禁用INTR字符
    // 检查并获取标准输入流的vdisable值
    // vdisable用于控制终端的特殊字符,如Ctrl+C等
    // 如果fpathconf函数调用失败,或者_POSIX_VDISABLE未生效,则输出错误信息并退出程序
    if ((vdisable = fpathconf(STDIN_FILENO, _PC_VDISABLE)) < 0)
        err_quit("fpathconf error or _POSIX_VDISABLE not in effect");

    // 获取当前终端属性
    // tcgetattr函数用于获取终端属性
    // STDIN_FILENO 是标准输入的文件描述符
    // term 是一个指向termios结构体的指针,用于存储获取到的终端属性
    // 如果调用成功,返回0;如果失败,返回-1,并设置errno以指示错误
    if (tcgetattr(STDIN_FILENO, &term) < 0)
        // 如果tcgetattr调用失败,err_sys函数用于打印错误信息并退出程序
        err_sys("tcgetattr error");

    // 禁用INTR字符
    term.c_cc[VINTR] = vdisable;
    // 将EOF设置为Control-B
    term.c_cc[VEOF] = 2;

    // 设置新的终端属性并立即生效
    // 设置终端为非规范模式,这样每次输入一个字符后都会立即返回,无需按下回车键.
    // STDIN_FILENO 是标准输入的文件描述符.
    // TCSAFLUSH 是一个标志,用于在设置终端属性时刷新输出队列.
    // &term 是指向终端属性结构的指针.
    // 如果 tcsetattr 函数调用失败,返回值小于 0,将调用 err_sys 函数报告错误.
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &term) < 0)
        err_sys("tcsetattr error");

    // 正常退出程序
    exit(0);
}
