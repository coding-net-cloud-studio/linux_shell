#include "apue.h"  // 引入APUE库的头文件
#include <time.h>  // 引入标准时间库的头文件

int main(void)
{
    time_t     caltime;        // 定义一个time_t类型的变量,用于存储当前日历时间
    struct tm *tm;             // 定义一个指向tm结构体的指针,用于存储分解后的时间
    char       line[MAXLINE];  // 定义一个字符数组,用于存储格式化后的时间字符串

    if ((caltime = time(NULL)) == -1)  // 获取当前日历时间,如果失败则返回-1
        err_sys("time error");         // 如果获取时间失败,调用err_sys函数输出错误信息并退出程序

    if ((tm = localtime(&caltime)) == NULL)  // 将日历时间转换为本地时间,并存储在tm指针指向的结构体中,如果失败则返回NULL
        err_sys("localtime error");          // 如果转换时间失败,调用err_sys函数输出错误信息并退出程序

    if (strftime(line, MAXLINE, "%a %b %d %X %Z %Y\n", tm) == 0)  // 将tm结构体中的时间格式化为指定的字符串格式,并存储在line数组中,如果格式化失败则返回0
        err_sys("strftime error");                                // 如果格式化时间失败,调用err_sys函数输出错误信息并退出程序

    fputs(line, stdout);  // 将格式化后的时间字符串输出到标准输出
    exit(0);              // 程序正常退出
}
