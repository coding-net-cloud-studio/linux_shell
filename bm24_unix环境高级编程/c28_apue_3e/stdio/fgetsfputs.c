#include "apue.h"

// 主函数
int main(void)
{
    // 定义一个字符数组buf,用于存储从标准输入读取的数据
    char buf[MAXLINE];

    // 使用fgets函数从标准输入读取数据,直到遇到换行符或EOF
    while (fgets(buf, MAXLINE, stdin) != NULL)
        // 使用fputs函数将读取到的数据写入标准输出
        // 如果写入失败,调用err_sys函数报告输出错误
        if (fputs(buf, stdout) == EOF)
            err_sys("output error");

    // 检查标准输入是否有错误发生,如果有,调用err_sys函数报告输入错误
    if (ferror(stdin))
        err_sys("input error");

    // 程序正常退出
    exit(0);
}
