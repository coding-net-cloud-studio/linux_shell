#include "apue.h"

// 全局变量,在初始化数据段中
int globvar = 6;
// 缓冲区,用于写入标准输出
char buf[] = "a write to stdout\n";

int main(void)
{
    // 栈上的自动变量
    int var;
    // 进程ID类型
    pid_t pid;

    // 设置局部变量的值
    var = 88;
    // 尝试将缓冲区内容写入标准输出,如果不等于缓冲区大小减一,则报告错误
    if (write(STDOUT_FILENO, buf, sizeof(buf) - 1) != sizeof(buf) - 1)
        err_sys("write error");
    // 在fork之前打印信息,注意这里没有刷新标准输出
    printf("before fork\n");

    // 创建子进程
    if ((pid = fork()) < 0)
    {
        // 如果fork失败,报告错误
        err_sys("fork error");
    }
    else if (pid == 0)
    {              /* 子进程 */
        globvar++; /* 修改全局变量 */
        var++;     /* 修改局部变量 */
    }
    else
    {
        sleep(2); /* 父进程等待2秒 */
    }

    // 打印进程ID,全局变量和局部变量的值
    printf("pid = %ld, glob = %d, var = %d\n", (long)getpid(), globvar, var);
    // 退出进程
    exit(0);
}
