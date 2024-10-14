#include "apue.h"

// 全局变量,在初始化数据段中
int globvar = 6;

int main(void)
{
    // 栈上的自动变量
    int var;
    // 进程ID类型
    pid_t pid;

    // 给局部变量赋值
    var = 88;
    // 打印信息,注意这里没有刷新stdio
    printf("before vfork\n");
    // 使用vfork创建子进程
    if ((pid = vfork()) < 0)
    {
        // 如果vfork失败,打印错误信息并退出
        err_sys("vfork error");
    }
    else if (pid == 0)
    {               // 子进程
        globvar++;  // 修改父进程的全局变量
        var++;      // 修改子进程的局部变量
        _exit(0);   // 子进程终止
    }

    // 父进程继续执行
    printf("pid = %ld, glob = %d, var = %d\n", (long)getpid(), globvar, var);
    exit(0);
}
