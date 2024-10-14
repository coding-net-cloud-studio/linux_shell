#include "apue.h"

// do_driver函数用于创建一个子进程来执行指定的驱动程序,并通过全双工管道与其通信.
void do_driver(char *driver)
{
    pid_t child;    // 子进程ID
    int   pipe[2];  // 管道文件描述符数组

    /*
     * 创建一个全双工管道以与驱动程序通信.
     */
    if (fd_pipe(pipe) < 0)                    // 创建管道失败
        err_sys("can't create stream pipe");  // 输出错误信息

    if ((child = fork()) < 0)  // 创建子进程失败
    {
        err_sys("fork error");  // 输出错误信息
    }
    else if (child == 0)  // 子进程执行的代码块
    {                     /* child */
        close(pipe[1]);   // 关闭子进程的写端

        /* 将管道的读端作为驱动程序的标准输入 */
        if (dup2(pipe[0], STDIN_FILENO) != STDIN_FILENO)
            err_sys("dup2 error to stdin");  // 输出错误信息

        /* 将管道的读端作为驱动程序的标准输出 */
        if (dup2(pipe[0], STDOUT_FILENO) != STDOUT_FILENO)
            err_sys("dup2 error to stdout");  // 输出错误信息
        if (pipe[0] != STDIN_FILENO && pipe[0] != STDOUT_FILENO)
            close(pipe[0]);  // 关闭多余的管道读端

        /* 让驱动程序的标准错误保持原样 */
        execlp(driver, driver, (char *)0);        // 执行驱动程序
        err_sys("execlp error for: %s", driver);  // 如果execlp失败,输出错误信息
    }

    close(pipe[0]);  // 父进程关闭读端

    /* 将管道的写端作为父进程的标准输入和输出 */
    if (dup2(pipe[1], STDIN_FILENO) != STDIN_FILENO)
        err_sys("dup2 error to stdin");  // 输出错误信息
    if (dup2(pipe[1], STDOUT_FILENO) != STDOUT_FILENO)
        err_sys("dup2 error to stdout");  // 输出错误信息
    if (pipe[1] != STDIN_FILENO && pipe[1] != STDOUT_FILENO)
        close(pipe[1]);  // 关闭多余的管道写端

    /*
     * 父进程返回,但标准输入和标准输出已连接到驱动程序.
     */
}
