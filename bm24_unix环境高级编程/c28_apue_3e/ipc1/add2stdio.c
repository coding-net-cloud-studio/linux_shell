#include "apue.h"   // 引入apue库,提供系统调用的封装和错误处理函数

// 主函数,程序入口
int main(void)
{
    int  int1, int2;     // 定义两个整型变量用于存储输入的整数
    char line[MAXLINE];  // 定义一个字符数组用于存储从标准输入读取的一行数据

    // 循环读取标准输入的每一行
    while (fgets(line, MAXLINE, stdin) != NULL)
    {
        // 尝试从读取的行中解析两个整数
        if (sscanf(line, "%d%d", &int1, &int2) == 2)
        {
            // 如果成功解析两个整数,打印它们的和
            if (printf("%d\n", int1 + int2) == EOF)
                err_sys("printf error");  // 如果打印出错,调用err_sys函数报告错误
        }
        else
        {
            // 如果未能解析两个整数,打印错误信息
            if (printf("invalid args\n") == EOF)
                err_sys("printf error");  // 如果打印出错,调用err_sys函数报告错误
        }
    }
    exit(0);  // 程序正常退出
}
