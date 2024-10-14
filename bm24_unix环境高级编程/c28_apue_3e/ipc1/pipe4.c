#include "apue.h"

// 信号处理函数,用于处理 SIGPIPE 信号
static void sig_pipe(int);

int main(void)
{
    int   n, fd1[2], fd2[2];  // n 用于存储读取的字节数,fd1 和 fd2 是管道文件描述符数组
    pid_t pid;                // 进程 ID
    char  line[MAXLINE];      // 用于存储从标准输入读取的行

    // 设置 SIGPIPE 信号的处理函数为 sig_pipe
    if (signal(SIGPIPE, sig_pipe) == SIG_ERR)
        err_sys("signal error");

    // 创建两个管道
    if (pipe(fd1) < 0 || pipe(fd2) < 0)
        err_sys("pipe error");

    // 创建子进程
    if ((pid = fork()) < 0)
    {
        err_sys("fork error");
    }
    else if (pid > 0)
    {                   /* 父进程 */
        close(fd1[0]);  // 关闭父进程不需要的管道读端
        close(fd2[1]);  // 关闭父进程不需要的管道写端

        // 从标准输入读取数据并写入管道 fd1,然后从管道 fd2 读取数据并写入标准输出
        // 从标准输入读取数据,通过管道发送到子进程,并从子进程接收处理后的数据输出到标准输出
        while (fgets(line, MAXLINE, stdin) != NULL)
        {                                               // 从标准输入读取一行数据
            n = strlen(line);                           // 获取读取到的数据的长度
            if (write(fd1[1], line, n) != n)            // 将数据写入管道
                err_sys("write error to pipe");         // 写入错误处理
            if ((n = read(fd2[0], line, MAXLINE)) < 0)  // 从管道读取子进程处理后的数据
                err_sys("read error from pipe");        // 读取错误处理
            if (n == 0)
            {                                  // 如果读取到的数据长度为0,表示子进程已关闭管道
                err_msg("child closed pipe");  // 提示子进程关闭管道
                break;                         // 结束循环
            }
            line[n] = 0;                     // 字符串结尾添加空字符,确保字符串正确结束
            if (fputs(line, stdout) == EOF)  // 将处理后的数据输出到标准输出
                err_sys("fputs error");      // 输出错误处理
        }

        if (ferror(stdin))                    // 检查标准输入是否有错误
            err_sys("fgets error on stdin");  // 输入错误处理
        exit(0);                              // 正常退出程序
    }
    else
    {                   /* 子进程 */
        close(fd1[1]);  // 关闭子进程不需要的管道写端
        close(fd2[0]);  // 关闭子进程不需要的管道读端
        if (fd1[0] != STDIN_FILENO)
        {
            // 将管道 fd1 的读端复制到标准输入
            if (dup2(fd1[0], STDIN_FILENO) != STDIN_FILENO)
                err_sys("dup2 error to stdin");
            close(fd1[0]);
        }

        if (fd2[1] != STDOUT_FILENO)
        {
            // 将管道 fd2 的写端复制到标准输出
            if (dup2(fd2[1], STDOUT_FILENO) != STDOUT_FILENO)
                err_sys("dup2 error to stdout");
            close(fd2[1]);
        }
        // 执行 add2 程序
        if (execl("./add2", "add2", (char *)0) < 0)
            err_sys("execl error");
    }
    exit(0);
}

// SIGPIPE 信号处理函数
// sig_pipe 函数用于处理 SIGPIPE 信号.
// 当程序向一个已经关闭的 socket 写数据时,系统会发送 SIGPIPE 信号给进程.
// 通常情况下,这会导致进程终止,但我们可以捕获这个信号并处理它.
static void
sig_pipe(int signo)
{
    // 打印消息表示捕获到了 SIGPIPE 信号.
    printf("SIGPIPE caught\n");
    // 退出程序,返回状态码 1.
    exit(1);
}
