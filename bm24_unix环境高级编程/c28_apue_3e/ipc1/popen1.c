#include "apue.h"      // 引入APUE库,提供系统调用和错误处理的函数
#include <sys/wait.h>  // 引入wait系统调用的头文件

int main(void)
{
    char  line[MAXLINE];  // 定义一个字符数组用于存储从管道读取的数据
    FILE *fpin;           // 定义一个FILE指针,用于指向popen创建的管道

    // 使用popen函数创建一个管道,并执行命令"myuclc","r"表示以只读模式打开管道
    if ((fpin = popen("myuclc", "r")) == NULL)
        err_sys("popen error");  // 如果popen失败,调用err_sys打印错误信息并退出程序

    for (;;)
    {
        fputs("prompt> ", stdout);  // 在标准输出打印提示符
        fflush(stdout);             // 刷新标准输出缓冲区,确保提示符立即显示
        // 从管道中读取一行数据到line数组,如果读取失败(如管道关闭),则跳出循环
        if (fgets(line, MAXLINE, fpin) == NULL)
            break;
        // 将读取到的数据输出到标准输出,如果输出失败,调用err_sys打印错误信息并退出程序
        if (fputs(line, stdout) == EOF)
            err_sys("fputs error to pipe");
    }
    // 关闭管道,如果关闭失败,调用err_sys打印错误信息并退出程序
    if (pclose(fpin) == -1)
        err_sys("pclose error");
    putchar('\n');  // 在标准输出打印换行符
    exit(0);        // 正常退出程序
}
