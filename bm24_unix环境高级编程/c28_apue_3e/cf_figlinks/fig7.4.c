#include "apue.h"

// <summary>
// 主函数,用于打印命令行参数
// </summary>
// <param name="argc">命令行参数的数量</param>
// <param name="argv">指向命令行参数字符串数组的指针</param>
int main(int argc, char *argv[])
{
    int i;

    // 遍历所有命令行参数并打印
    // 以下代码段用于打印出命令行参数
    // 参数argc表示命令行参数的个数,包括程序名本身
    // 参数argv是一个指向字符指针的数组,每个指针指向一个命令行参数
    for (i = 0; i < argc; i++) /* echo all command-line args */
        // 使用printf函数打印每个参数的索引和对应的字符串值
        printf("argv[%d]: %s\n", i, argv[i]);

    // 正常退出程序
    exit(0);
}
