#include "apue.h"

#define BUFFSIZE 512

// 信号处理函数,用于捕获SIGTERM信号
static void sig_term(int signo);

// 用于标记是否捕获到信号的原子变量
static volatile sig_atomic_t sigcaught;

/*
 * loop函数 - 主循环,负责从ptym读取数据并写入stdout,
 * 同时处理子进程发送的SIGTERM信号.
 *
 * @param ptym: pty主设备文件描述符
 * @param ignoreeof: 是否忽略stdin的EOF信号
 */
void loop(int ptym, int ignoreeof)
{
    pid_t child;          // 子进程ID
    int   nread;          // 读取的字节数
    char  buf[BUFFSIZE];  // 缓冲区

    // 创建子进程
    if ((child = fork()) < 0)
    {
        err_sys("fork error");  // 创建子进程失败
    }
    else if (child == 0)
    { /* 子进程从stdin读取数据并写入ptym */
        for (;;)
        {
            if ((nread = read(STDIN_FILENO, buf, BUFFSIZE)) < 0)
                err_sys("read error from stdin");  // 从stdin读取失败
            else if (nread == 0)
                break;  // stdin遇到EOF,结束循环
            if (writen(ptym, buf, nread) != nread)
                err_sys("writen error to master pty");  // 写入ptym失败
        }

        // 如果ignoreeof为0,通知父进程
        if (ignoreeof == 0)
            kill(getppid(), SIGTERM);  // 发送SIGTERM信号给父进程
        exit(0);                       // 子进程退出
    }

    // 父进程从ptym读取数据并写入stdout
    if (signal_intr(SIGTERM, sig_term) == SIG_ERR)
        err_sys("signal_intr error for SIGTERM");  // 设置SIGTERM信号处理函数失败

    for (;;)
    {
        if ((nread = read(ptym, buf, BUFFSIZE)) <= 0)
            break;  // 读取失败或遇到EOF
        if (writen(STDOUT_FILENO, buf, nread) != nread)
            err_sys("writen error to stdout");  // 写入stdout失败
    }

    // 如果sigcaught为0,通知子进程停止
    if (sigcaught == 0)
        kill(child, SIGTERM);

    // 父进程返回
}

/*
 * sig_term函数 - 信号处理函数,当子进程遇到EOF或read()失败时,
 * 子进程会发送SIGTERM信号,此函数用于捕获该信号.
 *
 * @param signo: 信号编号
 */
static void sig_term(int signo)
{
    sigcaught = 1;  // 设置信号捕获标志
}
