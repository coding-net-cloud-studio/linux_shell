#include "apue.h"      // 引入APUE库,提供系统编程接口
#include <sys/wait.h>  // 引入wait系统调用相关头文件

int main(void)
{
    int status;  // 定义整型变量status,用于存储系统调用的返回状态

    // 调用system函数执行date命令,将返回状态赋值给status
    if ((status = system("date")) < 0)
        err_sys("system() error");  // 如果返回状态小于0,表示出错,调用err_sys打印错误信息

    pr_exit(status);  // 打印system调用date命令的退出状态

    // 调用system函数执行一个不存在的命令nosuchcommand
    if ((status = system("nosuchcommand")) < 0)
        err_sys("system() error");  // 如果返回状态小于0,表示出错,调用err_sys打印错误信息

    pr_exit(status);  // 打印system调用nosuchcommand命令的退出状态

    // 调用system函数执行who命令,后面跟了一个分号和exit 44,表示无论who命令结果如何,都会执行exit 44
    if ((status = system("who; exit 44")) < 0)
        err_sys("system() error");  // 如果返回状态小于0,表示出错,调用err_sys打印错误信息

    pr_exit(status);  // 打印system调用who; exit 44命令的退出状态

    exit(0);  // 程序正常退出
}
