#include "apue.h"

// 主函数,接收命令行参数和环境变量,并打印它们
int main(int argc, char *argv[])
{
    int           i;        // 循环计数器
    char        **ptr;      // 指向环境变量字符串数组的指针
    extern char **environ;  // 声明外部环境变量指针

    // 遍历命令行参数并打印
    for (i = 0; i < argc; i++) /* echo all command-line args */
        printf("argv[%d]: %s\n", i, argv[i]);

    // 遍历环境变量并打印
    for (ptr = environ; *ptr != 0; ptr++) /* and all env strings */
        printf("%s\n", *ptr);

    exit(0);  // 程序正常退出
}
