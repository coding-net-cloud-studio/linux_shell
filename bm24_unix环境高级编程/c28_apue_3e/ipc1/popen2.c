#include "apue.h"      // 引入APUE库,提供系统调用和错误处理的函数
#include <sys/wait.h>  // 引入wait系统调用的头文件

// 定义PAGER环境变量,如果没有设置则默认为"more"
#define PAGER "${PAGER:-more}"

/**
 * 主函数
 * @param argc 命令行参数的个数
 * @param argv 命令行参数的数组
 * @return 程序退出状态码
 */
int main(int argc, char *argv[])
{
    char  line[MAXLINE];  // 用于存储从文件中读取的每一行
    FILE *fpin, *fpout;   // 文件指针,fpin指向输入文件,fpout指向输出管道

    // 检查命令行参数个数,如果不是2个(程序名+文件路径),则打印错误信息并退出
    if (argc != 2)
        err_quit("usage: a.out <pathname>");

    // 打开输入文件,如果失败则打印错误信息并退出
    if ((fpin = fopen(argv[1], "r")) == NULL)
        err_sys("can't open %s", argv[1]);

    // 使用popen函数创建一个管道,并执行PAGER命令,如果失败则打印错误信息并退出
    if ((fpout = popen(PAGER, "w")) == NULL)
        err_sys("popen error");

    // 将输入文件的内容复制到管道输出
    while (fgets(line, MAXLINE, fpin) != NULL)
    {
        // 将读取到的每一行写入到管道输出,如果失败则打印错误信息并退出
        if (fputs(line, fpout) == EOF)
            err_sys("fputs error to pipe");
    }

    // 检查fpin是否发生错误,如果有错误则打印错误信息并退出
    if (ferror(fpin))
        err_sys("fgets error");

    // 关闭管道并等待子进程结束,如果失败则打印错误信息并退出
    if (pclose(fpout) == -1)
        err_sys("pclose error");

    // 程序正常退出
    exit(0);
}
