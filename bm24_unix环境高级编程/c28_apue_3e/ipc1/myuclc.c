#include "apue.h"   // 引入apue库,提供系统调用的封装和错误处理函数
#include <ctype.h>  // 引入字符处理库,提供字符类型判断和转换函数

int main(void)
{
    int c;  // 定义一个整型变量c,用于存储从标准输入读取的字符

    // 使用while循环持续读取标准输入的字符,直到遇到文件结束符EOF
    while ((c = getchar()) != EOF)
    {
        if (isupper(c))      // 判断字符c是否为大写字母
            c = tolower(c);  // 如果是大写字母,则将其转换为小写字母

        // 将转换后的字符c输出到标准输出,如果输出失败则调用err_sys函数报告错误
        if (putchar(c) == EOF)
            err_sys("output error");

        if (c == '\n')       // 如果字符c是换行符
            fflush(stdout);  // 刷新标准输出缓冲区,确保输出立即显示
    }

    exit(0);  // 程序正常结束,退出进程
}
