#include "apue.h"  // 引入apue库,提供系统调用和标准C库函数的封装.

/**
 * @brief 执行一个驱动程序,并将其标准输入和输出重定向到管道.
 *
 * @param driver 要执行的驱动程序的名称.
 * @return 不返回任何值.
 */
void do_driver(char *driver)
{
    // 声明一个进程ID类型的变量,用于存储子进程的ID
    pid_t child;
    // 声明一个整型数组,用于存储管道的文件描述符
    int pipe[2];

    /*
     * 创建一个全双工管道,用于与驱动程序通信.
     */
    if (fd_pipe(pipe) < 0)
        // 如果创建管道失败,调用 err_sys 函数输出错误信息并终止程序
        err_sys("can't create stream pipe");

    // 创建子进程
    if ((child = fork()) < 0)
    {
        // 如果 fork 失败,调用 err_sys 函数输出错误信息并终止程序
        err_sys("fork error");
    }
    else if (child == 0)
    { /* 子进程 */
        // 关闭管道的写端
        close(pipe[1]);

        /* 将管道的读端复制到标准输入 */
        if (dup2(pipe[0], STDIN_FILENO) != STDIN_FILENO)
            // 如果复制失败,调用 err_sys 函数输出错误信息并终止程序
            err_sys("dup2 error to stdin");

        /* 将管道的读端复制到标准输出 */
        if (dup2(pipe[0], STDOUT_FILENO) != STDOUT_FILENO)
            // 如果复制失败,调用 err_sys 函数输出错误信息并终止程序
            err_sys("dup2 error to stdout");
        // 如果管道的读端不是标准输入或标准输出,则关闭它
        if (pipe[0] != STDIN_FILENO && pipe[0] != STDOUT_FILENO)
            close(pipe[0]);

        /* 保持驱动程序的标准错误输出不变 */
        execlp(driver, driver, (char *)0);
        // 如果 exec 失败,调用 err_sys 函数输出错误信息并终止程序
        err_sys("execlp error for: %s", driver);
    }

    // 父进程关闭管道的读端
    close(pipe[0]);
    // 将管道的写端复制到标准输入
    if (dup2(pipe[1], STDIN_FILENO) != STDIN_FILENO)
        // 如果复制失败,调用 err_sys 函数输出错误信息并终止程序
        err_sys("dup2 error to stdin");
    // 将管道的写端复制到标准输出
    if (dup2(pipe[1], STDOUT_FILENO) != STDOUT_FILENO)
        // 如果复制失败,调用 err_sys 函数输出错误信息并终止程序
        err_sys("dup2 error to stdout");
    // 如果管道的写端不是标准输入或标准输出,则关闭它
    if (pipe[1] != STDIN_FILENO && pipe[1] != STDOUT_FILENO)
        close(pipe[1]);

    /*
     * 父进程返回,但标准输入和标准输出已连接到驱动程序.
     */
}
