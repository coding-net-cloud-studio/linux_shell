#include "apue.h"

// @brief 主函数,用于打印命令行参数
// @param argc 命令行参数的数量
// @param argv 指向命令行参数的指针数组
int main(int argc, char *argv[])
{
    int i;  // 循环变量

    // 遍历所有命令行参数并打印
    for (i = 0; i < argc; i++) /* echo all command-line args */
        printf("argv[%d]: %s\n", i, argv[i]);

    exit(0);  // 程序正常退出
}
