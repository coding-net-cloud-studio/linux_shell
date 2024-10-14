#include "apue.h"
#include <sys/wait.h>

// main函数用于执行系统命令并打印命令的退出状态
int main(void)
{
    int status;

    // 执行date命令,如果执行失败则打印错误信息
    if ((status = system("date")) < 0)
        err_sys("system() error");

    // 打印date命令的退出状态
    pr_exit(status);

    // 执行一个不存在的命令nosuchcommand,用于演示错误处理
    if ((status = system("nosuchcommand")) < 0)
        err_sys("system() error");

    // 打印nosuchcommand命令的退出状态
    pr_exit(status);

    // 执行who命令后跟exit 44,用于演示如何获取特定退出状态
    if ((status = system("who; exit 44")) < 0)
        err_sys("system() error");

    // 打印who命令和exit 44的退出状态
    pr_exit(status);

    // 正常退出程序
    exit(0);
}
