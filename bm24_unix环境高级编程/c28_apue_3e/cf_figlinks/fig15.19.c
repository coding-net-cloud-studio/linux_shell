#include "apue.h"

// 主函数
int main(void)
{
    int  int1, int2;     // 定义两个整型变量用于存储输入的数字
    char line[MAXLINE];  // 定义一个字符数组用于存储从标准输入读取的一行数据

    // 循环读取标准输入的每一行
    while (fgets(line, MAXLINE, stdin) != NULL)
    {
        // 尝试从读取的行中解析两个整数
        if (sscanf(line, "%d%d", &int1, &int2) == 2)
        {
            // 如果成功解析两个整数,则输出它们的和
            if (printf("%d\n", int1 + int2) == EOF)
                err_sys("printf error");  // 如果输出错误,则调用err_sys函数报告错误
        }
        else
        {
            // 如果解析失败,则输出"invalid args"
            if (printf("invalid args\n") == EOF)
                err_sys("printf error");  // 如果输出错误,则调用err_sys函数报告错误
        }
    }
    exit(0);  // 程序正常退出
}
