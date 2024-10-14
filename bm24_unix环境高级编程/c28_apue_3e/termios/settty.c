#include "apue.h"
#include <termios.h>

// 主函数
int main(void)
{
    struct termios term;      // 定义终端属性结构体
    long           vdisable;  // 定义变量用于存储禁用字符的值

    // 检查标准输入是否为终端设备
    if (isatty(STDIN_FILENO) == 0)
        err_quit("standard input is not a terminal device");

    // 获取_POSIX_VDISABLE的值,该值用于禁用终端输入字符
    if ((vdisable = fpathconf(STDIN_FILENO, _PC_VDISABLE)) < 0)
        err_quit("fpathconf error or _POSIX_VDISABLE not in effect");

    // 获取终端当前属性
    if (tcgetattr(STDIN_FILENO, &term) < 0)
        err_sys("tcgetattr error");

    // 禁用INTR字符
    term.c_cc[VINTR] = vdisable;
    // 设置EOF为Control-B
    term.c_cc[VEOF] = 2;

    // 设置终端属性并刷新输出缓冲区
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &term) < 0)
        err_sys("tcsetattr error");

    // 正常退出程序
    exit(0);
}
