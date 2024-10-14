#include "apue.h"      // 引入自定义的apue库头文件
#include <sys/wait.h>  // 引入系统等待子进程的头文件

// 定义PAGER环境变量,如果环境变量未设置,则使用默认值"more"
// ${PAGER:-more} 是一种shell参数替换,用于获取环境变量PAGER的值,如果未设置则返回"more"
#define PAGER "${PAGER:-more}"
// fig15.11.c

/**
 * 主函数,用于将指定文件的内容通过分页器输出.
 * @param argc 命令行参数的数量
 * @param argv 命令行参数的数组
 * @return 程序退出状态码
 */
int main(int argc, char *argv[])
{
    char  line[MAXLINE];  // 用于存储从文件中读取的行
    FILE *fpin, *fpout;   // 文件指针,分别指向输入文件和分页器进程

    // 检查命令行参数数量是否正确
    if (argc != 2)
        err_quit("usage: a.out <pathname>");  // 参数数量不正确,输出使用方法并退出

    // 打开输入文件
    if ((fpin = fopen(argv[1], "r")) == NULL)
        err_sys("can't open %s", argv[1]);  // 文件打开失败,输出错误信息并退出

    // 打开分页器进程
    // 使用popen函数打开一个分页器进程,将输出重定向到该进程
    // PAGER是一个环境变量,通常指向用户默认的分页器程序,如less或more
    // "w"表示以写入模式打开,即向分页器进程发送数据
    if ((fpout = popen(PAGER, "w")) == NULL)
        err_sys("popen error");  // 如果分页器进程打开失败,则调用err_sys函数输出错误信息并退出程序

    /* 将输入文件的内容复制到分页器 */
    while (fgets(line, MAXLINE, fpin) != NULL)  // 逐行读取文件内容
    {
        if (fputs(line, fpout) == EOF)       // 将读取到的行写入分页器进程
            err_sys("fputs error to pipe");  // 写入失败,输出错误信息并退出
    }

    // 检查文件读取过程中是否发生错误
    if (ferror(fpin))
        err_sys("fgets error");  // 发生读取错误,输出错误信息并退出

    // 关闭分页器进程
    if (pclose(fpout) == -1)
        err_sys("pclose error");  // 关闭进程失败,输出错误信息并退出

    exit(0);  // 程序正常退出
}
