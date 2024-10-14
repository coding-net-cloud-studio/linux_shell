#include "apue.h"

// 主函数,用于从标准输入读取字符并输出到标准输出
int main(void)
{
    int c;  // 用于存储读取的字符

    // 循环读取标准输入的字符,直到遇到EOF(文件结束符)
    while ((c = getc(stdin)) != EOF)
        // 将读取到的字符输出到标准输出,如果输出失败则调用err_sys报告错误
        if (putc(c, stdout) == EOF)
            err_sys("output error");

    // 检查标准输入是否有错误发生,如果有则调用err_sys报告错误
    if (ferror(stdin))
        err_sys("input error");

    // 程序正常退出
    exit(0);
}
