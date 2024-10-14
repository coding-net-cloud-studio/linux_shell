#include "apue.h"

// <summary>
// 处理 SIGINT 信号的函数
// </summary>
// <param name="signo">信号编号</param>
static void
sig_int(int signo)
{
    printf("caught SIGINT\n");
}

// <summary>
// 处理 SIGCHLD 信号的函数
// </summary>
// <param name="signo">信号编号</param>
static void
sig_chld(int signo)
{
    printf("caught SIGCHLD\n");
}

// <summary>
// 主函数,设置信号处理程序并执行系统命令
// </summary>
// <returns>程序退出状态码</returns>
int main(void)
{
    // 设置 SIGINT 信号的处理程序为 sig_int 函数
    if (signal(SIGINT, sig_int) == SIG_ERR)
        err_sys("signal(SIGINT) error");

    // 设置 SIGCHLD 信号的处理程序为 sig_chld 函数
    if (signal(SIGCHLD, sig_chld) == SIG_ERR)
        err_sys("signal(SIGCHLD) error");

    // 执行 /bin/ed 命令,如果执行失败则输出错误信息
    if (system("/bin/ed") < 0)
        err_sys("system() error");

    // 正常退出程序
    exit(0);
}
