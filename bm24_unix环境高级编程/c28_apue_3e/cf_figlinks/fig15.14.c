#include "apue.h"   // 引入APUE库的头文件
#include <ctype.h>  // 引入字符处理函数库

int main(void)
{
    int c;  // 定义一个整型变量c,用于存储从getchar()读取的字符

    // 循环读取字符,直到遇到文件结束符EOF
    while ((c = getchar()) != EOF)
    {
        if (isupper(c))               // 如果读取的字符是大写字母
            c = tolower(c);           // 将其转换为小写字母
        if (putchar(c) == EOF)        // 尝试输出字符,如果输出失败则返回EOF
            err_sys("output error");  // 输出错误信息
        if (c == '\n')                // 如果读取的字符是换行符
            fflush(stdout);           // 刷新标准输出缓冲区
    }
    exit(0);  // 正常退出程序
}
