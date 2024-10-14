#include "apue.h"

// 全局变量,在初始化数据段中
int globvar = 6;

// 缓冲区,用于写入标准输出的内容
char buf[] = "a write to stdout\n";

int main(void)
{
    int   var;  // 栈上的自动变量
    pid_t pid;  // 进程ID

    var = 88;  // 初始化局部变量var

    // 将buf的内容写入标准输出,如果写入的字节数不等于buf的大小减1,则报告错误
    if (write(STDOUT_FILENO, buf, sizeof(buf) - 1) != sizeof(buf) - 1)
        err_sys("write error");

    printf("before fork\n");  // 在fork之前打印信息,注意这里没有刷新标准输出

    // 创建子进程
    if ((pid = fork()) < 0)
    {
        err_sys("fork error");  // 如果fork失败,报告错误
    }
    else if (pid == 0)
    {               // 子进程
        globvar++;  // 修改全局变量
        var++;      // 修改局部变量
    }
    else
    {              // 父进程
        sleep(2);  // 父进程休眠2秒
    }

    // 打印进程ID,全局变量和局部变量的值
    // 打印当前进程的PID,全局变量globvar的值,以及局部变量var的值
    // 使用getpid()函数获取当前进程的PID,该函数返回类型为pid_t
    // globvar是一个全局变量,var是一个局部变量
    printf("pid = %ld, glob = %d, var = %d\n", (long)getpid(), globvar, var);  // 格式化输出

    exit(0);  // 退出程序
}
