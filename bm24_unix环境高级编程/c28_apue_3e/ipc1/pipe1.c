#include "apue.h"

int main(void)
{
    int   n;              // 用于存储读取的字节数
    int   fd[2];          // 文件描述符数组,用于存储管道的两端
    pid_t pid;            // 进程ID
    char  line[MAXLINE];  // 缓冲区,用于存储从管道读取的数据

    if (pipe(fd) < 0)           // 创建管道
        err_sys("pipe error");  // 如果创建失败,打印错误并退出
    if ((pid = fork()) < 0)     // 创建子进程
    {
        err_sys("fork error");  // 如果创建失败,打印错误并退出
    }
    else if (pid > 0)
    { /* parent */                          // 父进程
        close(fd[0]);                       // 关闭管道的读端
        write(fd[1], "hello world\n", 12);  // 向管道的写端写入数据
    }
    else
    { /* child */                        // 子进程
        close(fd[1]);                    // 关闭管道的写端
        n = read(fd[0], line, MAXLINE);  // 从管道的读端读取数据到缓冲区
        write(STDOUT_FILENO, line, n);   // 将缓冲区的数据写入标准输出
    }
    exit(0);  // 退出进程
}
