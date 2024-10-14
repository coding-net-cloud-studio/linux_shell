// 引入所需的头文件
#include "apue.h"
#include <time.h>

// 主函数,程序的入口点
int main(void)
{
    // 定义一个 time_t 类型的变量,用于存储当前时间
    time_t caltime;
    // 定义一个指向 struct tm 结构体的指针,用于存储本地时间
    struct tm *tm;
    // 定义一个字符数组,用于存储格式化后的时间字符串
    char line[MAXLINE];

    // 调用 time 函数获取当前时间,如果返回 -1 表示获取时间失败
    if ((caltime = time(NULL)) == -1)
        // 调用 err_sys 函数输出错误信息并终止程序
        err_sys("time error");
    // 调用 localtime 函数将当前时间转换为本地时间,如果返回 NULL 表示转换失败
    if ((tm = localtime(&caltime)) == NULL)
        // 调用 err_sys 函数输出错误信息并终止程序
        err_sys("localtime error");
    // 调用 strftime 函数将本地时间格式化为字符串,如果返回 0 表示格式化失败
    if (strftime(line, MAXLINE, "%a %b %d %X %Z %Y\n", tm) == 0)
        // 调用 err_sys 函数输出错误信息并终止程序
        err_sys("strftime error");
    // 将格式化后的时间字符串输出到标准输出
    fputs(line, stdout);
    // 程序正常结束,返回 0
    exit(0);
}
