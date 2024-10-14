#include "apue.h"

// 主函数
int main(void)
{
    int c;  // 定义字符变量c

    // 循环读取标准输入的每个字符,直到遇到文件结束符EOF
    while ((c = getc(stdin)) != EOF)
        // 将读取到的字符c写入到标准输出,如果写入失败则调用err_sys函数报告错误
        if (putc(c, stdout) == EOF)
            err_sys("output error");

    // 检查标准输入是否有错误发生,如果有则调用err_sys函数报告错误
    if (ferror(stdin))
        err_sys("input error");

    // 程序正常退出
    exit(0);
}
