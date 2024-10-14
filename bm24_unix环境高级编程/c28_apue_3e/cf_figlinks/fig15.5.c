#include "apue.h"  // 引入 apue.h 头文件,该头文件包含程序所需的 API 函数声明和定义.

// 主函数
int main(void)
{
    int   n;              // 用于存储读取的字节数
    int   fd[2];          // 文件描述符数组,用于pipe
    pid_t pid;            // 进程ID
    char  line[MAXLINE];  // 缓冲区,用于存储从管道读取的数据

    // 创建管道,fd[0]用于读取,fd[1]用于写入
    if (pipe(fd) < 0)
        err_sys("pipe error");

    // 创建子进程
    if ((pid = fork()) < 0)
    {
        err_sys("fork error");  // 如果fork失败,打印错误信息并退出
    }
    else if (pid > 0)
    {                                       // 父进程
        close(fd[0]);                       // 关闭父进程的读端
        write(fd[1], "hello world\n", 12);  // 向管道写入数据
    }
    else
    {                                    // 子进程
        close(fd[1]);                    // 关闭子进程的写端
        n = read(fd[0], line, MAXLINE);  // 从管道读取数据到缓冲区
        write(STDOUT_FILENO, line, n);   // 将读取的数据写入标准输出
    }
    exit(0);  // 退出进程
}
