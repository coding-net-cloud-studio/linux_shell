#include "apue.h"

// 主函数,接收命令行参数并执行
int main(int argc, char *argv[])
{
    int status;  // 用于存储系统调用的返回状态

    // 检查是否提供了至少一个命令行参数

    // 检查命令行参数的数量
    if (argc < 2)
        // 如果参数少于2个,打印错误信息并退出程序
        err_quit("command-line argument required");

    // 使用system函数执行命令行参数指定的命令
    if ((status = system(argv[1])) < 0)
        err_sys("system() error");  // 如果执行出错,打印错误信息

    pr_exit(status);  // 打印退出状态

    exit(0);  // 正常退出程序
}
