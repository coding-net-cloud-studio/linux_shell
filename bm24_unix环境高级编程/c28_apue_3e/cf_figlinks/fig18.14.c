#include "apue.h"

// 主函数,用于检查标准输入输出流的文件描述符是否为终端
int main(void)
{
    // 检查标准输入(文件描述符0)是否为终端,如果是则输出"tty",否则输出"not a tty"
    printf("fd 0: %s\n", isatty(0) ? "tty" : "not a tty");

    // 检查标准输出(文件描述符1)是否为终端,如果是则输出"tty",否则输出"not a tty"
    printf("fd 1: %s\n", isatty(1) ? "tty" : "not a tty");

    // 检查标准错误输出(文件描述符2)是否为终端,如果是则输出"tty",否则输出"not a tty"
    printf("fd 2: %s\n", isatty(2) ? "tty" : "not a tty");

    // 程序正常退出
    exit(0);
}
