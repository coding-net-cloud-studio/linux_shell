#include "apue.h"      // 引入apue库,提供系统编程接口
#include <sys/wait.h>  // 引入sys/wait.h头文件,提供进程等待相关函数

// 主函数
int main(void)
{
    char  line[MAXLINE];  // 定义一个字符数组用于存储从管道读取的数据
    FILE *fpin;           // 定义一个文件指针用于指向管道

    // 使用popen函数创建一个管道,并执行命令"myuclc","r"表示读取模式
    if ((fpin = popen("myuclc", "r")) == NULL)
        err_sys("popen error");  // 如果管道创建失败,输出错误信息并退出程序

    // 无限循环,直到从管道读取的数据为空
    for (;;)
    {
        fputs("prompt> ", stdout);               // 输出提示符
        fflush(stdout);                          // 刷新输出缓冲区
        if (fgets(line, MAXLINE, fpin) == NULL)  // 从管道读取数据到line数组
            break;                               // 如果读取失败(通常是因为子进程结束),则跳出循环
        if (fputs(line, stdout) == EOF)          // 将读取到的数据输出到标准输出
            err_sys("fputs error to pipe");      // 如果输出失败,输出错误信息并退出程序
    }

    if (pclose(fpin) == -1)       // 关闭管道
        err_sys("pclose error");  // 如果关闭失败,输出错误信息并退出程序

    putchar('\n');  // 输出换行符
    exit(0);        // 正常退出程序
}
