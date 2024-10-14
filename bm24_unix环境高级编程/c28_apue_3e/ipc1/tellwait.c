#include "apue.h"

// 定义两个管道的文件描述符数组
static int pfd1[2], pfd2[2];

/**
 * TELL_WAIT - 创建两个管道用于父子进程间的通信
 */
void TELL_WAIT(void)
{
    // 创建两个管道,如果任何一个创建失败则报错
    if (pipe(pfd1) < 0 || pipe(pfd2) < 0)
        err_sys("pipe error");
}

/**
 * TELL_PARENT - 向父进程发送信号
 * @pid: 子进程的PID
 */
void TELL_PARENT(pid_t pid)
{
    // 向管道pfd2写入一个字符'c',表示子进程已完成某项工作
    if (write(pfd2[1], "c", 1) != 1)
        err_sys("write error");
}

/**
 * WAIT_PARENT - 等待父进程的信号
 */
void WAIT_PARENT(void)
{
    char c;

    // 从管道pfd1读取一个字符,如果读取失败则报错
    if (read(pfd1[0], &c, 1) != 1)
        err_sys("read error");

    // 如果读取到的字符不是'p',则表示收到的信号不正确
    if (c != 'p')
        err_quit("WAIT_PARENT: incorrect data");
}

/**
 * TELL_CHILD - 向子进程发送信号
 * @pid: 父进程的PID
 */
void TELL_CHILD(pid_t pid)
{
    // 向管道pfd1写入一个字符'p',表示父进程已完成某项工作
    if (write(pfd1[1], "p", 1) != 1)
        err_sys("write error");
}

/**
 * WAIT_CHILD - 等待子进程的信号
 */
void WAIT_CHILD(void)
{
    char c;

    // 从管道pfd2读取一个字符,如果读取失败则报错
    if (read(pfd2[0], &c, 1) != 1)
        err_sys("read error");

    // 如果读取到的字符不是'c',则表示收到的信号不正确
    if (c != 'c')
        err_quit("WAIT_CHILD: incorrect data");
}
