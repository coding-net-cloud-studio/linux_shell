#include "apue.h"

// 主函数
int main(void)
{
    char buf[MAXLINE];  // 定义一个字符数组用于存储从标准输入读取的数据

    // 循环读取标准输入的每一行数据
    while (fgets(buf, MAXLINE, stdin) != NULL)
        // 将读取到的数据写入到标准输出
        if (fputs(buf, stdout) == EOF)
            // 如果写入失败,调用err_sys函数报告错误
            err_sys("output error");

    // 如果读取过程中发生错误,调用err_sys函数报告错误
    if (ferror(stdin))
        err_sys("input error");

    // 程序正常退出
    exit(0);
}
