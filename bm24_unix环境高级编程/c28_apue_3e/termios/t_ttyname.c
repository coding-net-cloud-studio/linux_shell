#include "apue.h"

// 主函数
int main(void)
{
    char *name;  // 定义一个字符指针用于存储终端名称

    // 检查标准输入是否为终端
    if (isatty(0))
    {
        name = ttyname(0);  // 获取标准输入的终端名称
        if (name == NULL)
            name = "undefined";  // 如果获取失败,设置名称为"undefined"
    }
    else
    {
        name = "not a tty";  // 如果不是终端,设置名称为"not a tty"
    }
    printf("fd 0: %s\n", name);  // 打印标准输入的终端名称

    // 检查标准输出是否为终端
    if (isatty(1))
    {
        name = ttyname(1);  // 获取标准输出的终端名称
        if (name == NULL)
            name = "undefined";  // 如果获取失败,设置名称为"undefined"
    }
    else
    {
        name = "not a tty";  // 如果不是终端,设置名称为"not a tty"
    }
    printf("fd 1: %s\n", name);  // 打印标准输出的终端名称

    // 检查标准错误输出是否为终端
    if (isatty(2))
    {
        name = ttyname(2);  // 获取标准错误输出的终端名称
        if (name == NULL)
            name = "undefined";  // 如果获取失败,设置名称为"undefined"
    }
    else
    {
        name = "not a tty";  // 如果不是终端,设置名称为"not a tty"
    }
    printf("fd 2: %s\n", name);  // 打印标准错误输出的终端名称

    exit(0);  // 程序正常退出
}
