#include "apue.h"

// 主函数
int main(void)
{
    char *name;  // 用于存储终端名称的指针

    // 检查标准输入是否为终端
    if (isatty(0))
    {
        name = ttyname(0);       // 获取标准输入的终端名称
        if (name == NULL)        // 如果获取失败
            name = "undefined";  // 设置名称为未定义
    }
    else
    {
        name = "not a tty";  // 如果不是终端,设置名称为非终端
    }
    printf("fd 0: %s\n", name);  // 打印标准输入的终端名称

    // 检查标准输出是否为终端
    if (isatty(1))
    {
        name = ttyname(1);       // 获取标准输出的终端名称
        if (name == NULL)        // 如果获取失败
            name = "undefined";  // 设置名称为未定义
    }
    else
    {
        name = "not a tty";  // 如果不是终端,设置名称为非终端
    }
    printf("fd 1: %s\n", name);  // 打印标准输出的终端名称

    // 检查标准错误输出是否为终端
    if (isatty(2))
    {
        name = ttyname(2);       // 获取标准错误输出的终端名称
        if (name == NULL)        // 如果获取失败
            name = "undefined";  // 设置名称为未定义
    }
    else
    {
        name = "not a tty";  // 如果不是终端,设置名称为非终端
    }
    printf("fd 2: %s\n", name);  // 打印标准错误输出的终端名称

    exit(0);  // 程序正常退出
}
