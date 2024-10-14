#include "apue.h"  // 引入apue库,提供系统调用和标准C库函数的封装

// 定义缓冲区大小
#define BUFFSIZE 512

// 信号处理函数声明,用于处理终止信号
static void sig_term(int);

// 定义一个volatile sig_atomic_t类型的变量,用于标记信号是否被捕获
// sig_atomic_t是一个保证在信号处理函数中可以安全访问的整数类型
static volatile sig_atomic_t sigcaught; /* set by signal handler */

void loop(int ptym, int ignoreeof)
{

    // 定义子进程的pid
    pid_t child;

    // 定义读取的字节数
    int nread;

// 定义缓冲区大小
#define BUFFSIZE 4096
    char buf[BUFFSIZE];

    // 使用fork()函数创建子进程
    // 如果fork()返回值小于0,表示创建子进程失败
    if ((child = fork()) < 0)
    {
        // 调用err_sys函数输出错误信息
        err_sys("fork error");
    }

    else if (child == 0)
    { /* child copies stdin to ptym */
        // 无限循环,用于从标准输入读取数据并写入到伪终端主设备
        for (;;)
        {
            // 从标准输入读取数据到缓冲区buf,BUFFSIZE为缓冲区大小
            // 如果读取的字节数小于0,表示发生错误,调用err_sys函数报告错误
            if ((nread = read(STDIN_FILENO, buf, BUFFSIZE)) < 0)
                err_sys("read error from stdin");
            // 如果读取的字节数为0,表示标准输入已到达文件结束符EOF,退出循环
            else if (nread == 0)
                break; /* EOF on stdin means we're done */
            // 将缓冲区buf中的数据写入到伪终端主设备ptym
            // 如果写入的字节数不等于读取的字节数,表示发生错误,调用err_sys函数报告错误
            if (writen(ptym, buf, nread) != nread)
                err_sys("writen error to master pty");
        }

        /*
         * We always terminate when we encounter an EOF on stdin,
         * but we notify the parent only if ignoreeof is 0.
         */

        /*
         * 当我们在标准输入(stdin)上遇到EOF时,我们总是终止程序,
         * 但如果ignoreeof为0,我们只通知父进程.
         */
        if (ignoreeof == 0)
        {
            // 如果ignoreeof为0,向父进程发送SIGTERM信号以通知它
            kill(getppid(), SIGTERM); /* notify parent */
        }
        // 终止当前进程;子进程不能返回
        exit(0);
    }

    /*
     * Parent copies ptym to stdout.
     */

    /*
     * 父进程将从伪终端读取的数据复制到标准输出
     */

    // 为SIGTERM信号注册处理函数sig_term
    // 如果注册失败,则通过err_sys函数报告错误
    if (signal_intr(SIGTERM, sig_term) == SIG_ERR)
        err_sys("signal_intr error for SIGTERM");

    // 无限循环,用于持续读取数据
    for (;;)
    {
        // 从伪终端读取数据到缓冲区buf,BUFFSIZE为缓冲区大小
        // 如果读取的字节数nread小于等于0,表示捕获到信号,发生错误或到达文件末尾,跳出循环
        if ((nread = read(ptym, buf, BUFFSIZE)) <= 0)
            break; /* signal caught, error, or EOF */

        // 将缓冲区buf中的数据写入标准输出(STDOUT_FILENO)
        // 如果写入的字节数不等于nread,表示写入发生错误,调用err_sys函数报告错误
        if (writen(STDOUT_FILENO, buf, nread) != nread)
            err_sys("writen error to stdout");
    }

    /*
     * There are three ways to get here: sig_term() below caught the
     * SIGTERM from the child, we read an EOF on the pty master (which
     * means we have to signal the child to stop), or an error.
     */

    /*
     * 有三种方式到达这里:
     * 1. 下面的 sig_term() 捕获到了来自子进程的 SIGTERM 信号.
     * 2. 在 pty 主设备上读取到了 EOF,这意味着我们需要向子进程发送信号以停止它.
     * 3. 发生了一个错误.
     */

    // 如果子进程没有发送信号给我们,告诉子进程终止
    if (sigcaught == 0)
        kill(child, SIGTERM);

    /*
     * 父进程返回给调用者.
     */
}

/*
 * The child sends us SIGTERM when it gets EOF on the pty slave or
 * when read() fails.  We probably interrupted the read() of ptym.
 */

/*
 * 子进程在 pty 从设备上遇到 EOF 或者 read() 失败时,会向我们发送 SIGTERM 信号.
 * 我们可能中断了对 pty 的 read() 操作.
 */

// 定义信号处理函数 sig_term
static void
sig_term(int signo)
{
    sigcaught = 1;  // 设置标志位并返回
    // 注释:该函数用于处理接收到的 SIGTERM 信号,当子进程发送此信号时,
    // 父进程会通过设置 sigcaught 标志位来响应,并进行相应的处理.
}
