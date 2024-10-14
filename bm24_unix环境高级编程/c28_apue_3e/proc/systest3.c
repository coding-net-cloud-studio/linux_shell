#include "apue.h"

// 主函数,接收命令行参数并执行系统命令
int main(int argc, char *argv[])
{
    int status;  // 用于存储系统命令的退出状态

    // 检查是否提供了至少一个命令行参数
    if (argc < 2)
        err_quit("command-line argument required");  // 如果没有提供参数,打印错误信息并退出程序

    // 执行命令行参数指定的系统命令
    if ((status = system(argv[1])) < 0)
        err_sys("system() error");  // 如果执行命令出错,打印错误信息并退出程序

    pr_exit(status);  // 打印系统命令的退出状态

    exit(0);  // 正常退出程序
}
