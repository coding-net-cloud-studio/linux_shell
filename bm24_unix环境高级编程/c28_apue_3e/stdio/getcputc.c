#include "apue.h"

// 主函数,程序入口点
int main(void)
{
    int c;  // 定义一个整型变量c,用于存储从标准输入读取的字符

    // 循环读取标准输入的字符,直到遇到EOF(文件结束符)
    while ((c = getc(stdin)) != EOF)
        // 将读取到的字符c写入标准输出,如果写入失败则调用err_sys函数报告输出错误
        if (putc(c, stdout) == EOF)
            err_sys("output error");

    // 检查标准输入是否有错误发生,如果有则调用err_sys函数报告输入错误
    if (ferror(stdin))
        err_sys("input error");

    // 程序正常结束,退出
    exit(0);
}
