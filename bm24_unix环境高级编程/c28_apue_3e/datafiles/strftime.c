#include <stdio.h>   // 包含标准输入输出库
#include <stdlib.h>  // 包含标准库函数
#include <time.h>    // 包含时间处理库

// 主函数
int main(void)
{
    time_t     t;         // 定义一个time_t类型的变量t,用于存储当前时间
    struct tm *tmp;       // 定义一个tm结构体指针tmp,用于存储时间的详细信息
    char       buf1[16];  // 定义一个长度为16的字符数组buf1,用于存储格式化后的时间字符串
    char       buf2[64];  // 定义一个长度为64的字符数组buf2,用于存储格式化后的时间字符串

    time(&t);             // 获取当前时间并存储到变量t中
    tmp = localtime(&t);  // 将t中的时间转换为本地时间,并存储到tmp指向的结构体中

    // 使用strftime函数尝试将时间格式化并存储到buf1中,如果返回值为0表示缓冲区太小
    if (strftime(buf1, 16, "time and date: %r, %a %b %d, %Y", tmp) == 0)
        printf("buffer length 16 is too small\n");  // 如果缓冲区太小,打印提示信息
    else
        printf("%s\n", buf1);  // 否则打印格式化后的时间字符串

    // 使用strftime函数尝试将时间格式化并存储到buf2中,如果返回值为0表示缓冲区太小
    if (strftime(buf2, 64, "time and date: %r, %a %b %d, %Y", tmp) == 0)
        printf("buffer length 64 is too small\n");  // 如果缓冲区太小,打印提示信息
    else
        printf("%s\n", buf2);  // 否则打印格式化后的时间字符串

    exit(0);  // 程序正常退出
}
