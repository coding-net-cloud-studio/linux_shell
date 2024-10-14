#include "apue.h"

// 主函数,程序入口点
int main(int argc, char *argv[])
{
    int           i;        // 用于循环的计数器
    char        **ptr;      // 指向环境变量字符串数组的指针
    extern char **environ;  // 声明外部变量,指向环境变量字符串数组

    // 遍历命令行参数并打印
    // 遍历命令行参数并打印每个参数的索引和值
    for (int i = 0; i < argc; i++)  // argc 是命令行参数的数量,argv 是参数数组
    {
        printf("argv[%d]: %s\n", i, argv[i]);  // 打印参数的索引和对应的字符串值
    }

    // 遍历环境变量并打印
    // 遍历环境变量数组,并打印每个环境变量的值
    for (ptr = environ; *ptr != 0; ptr++)  // 从环境变量数组的起始地址开始,直到遇到终止符0
    {
        printf("%s\n", *ptr);  // 打印当前环境变量的值,并换行
    }

    exit(0);  // 正常退出程序
}
