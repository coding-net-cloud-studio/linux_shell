#include <stdio.h>

// 主函数,程序的入口点
int main(void)
{
    char c;  // 定义一个字符变量c,用于存储从输入读取的字符

    // 循环读取输入,直到遇到文件结束符EOF
    while ((c = getchar()) != EOF)
        putchar(c);  // 将读取到的字符输出到标准输出
}
