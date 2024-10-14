#include "apue.h"   // 引入APUE库的头文件
#include <fcntl.h>  // 引入文件控制相关的头文件
#include <errno.h>  // 引入错误处理相关的头文件

// @brief 信号处理函数,用于处理中断信号
// @param signo 接收到的信号编号
void sigint(int signo)
{
    // 函数体为空,可能是用于后续添加信号处理逻辑
}

int main(void)
{
    // 定义三个进程ID变量和一个文件描述符变量
    pid_t pid1, pid2, pid3;  // 进程ID变量
    int   fd;                // 文件描述符变量

    // 设置标准输出流的缓冲模式为无缓冲,这样每次调用printf后都会立即输出
    setbuf(stdout, NULL);

    // 为SIGINT信号(通常是Ctrl+C产生的中断信号)注册处理函数sigint
    // 这样当程序收到SIGINT信号时,会调用sigint函数进行处理
    signal_intr(SIGINT, sigint);

    /*
     * 创建一个文件.
     */
    if ((fd = open("lockfile", O_RDWR | O_CREAT, 0666)) < 0)  // 打开或创建一个名为"lockfile"的文件,具有读写权限,如果不存在则创建
        err_sys("can't open/create lockfile");

    /*
     * 对文件进行读锁.
     */
    if ((pid1 = fork()) < 0)  // 创建子进程
    {
        err_sys("fork failed");  // 如果fork失败,打印错误信息
    }
    else if (pid1 == 0)
    {                                                            /* 子进程 */
        if (lock_reg(fd, F_SETLK, F_RDLCK, 0, SEEK_SET, 0) < 0)  // 尝试对文件进行读锁定
            err_sys("child 1: can't read-lock file");            // 如果锁定失败,打印错误信息
        printf("child 1: obtained read lock on file\n");         // 成功锁定后打印信息
        pause();                                                 // 暂停子进程,等待信号
        printf("child 1: exit after pause\n");                   // 暂停后打印退出信息
        exit(0);                                                 // 子进程退出
    }
    else
    {              /* 父进程 */
        sleep(2);  // 父进程休眠2秒,等待子进程锁定文件
    }

    /*
     * 父进程继续 ... 再次对文件进行读锁定.
     */
    if ((pid2 = fork()) < 0)  // 创建子进程,如果失败则返回错误
    {
        err_sys("fork failed");  // 输出错误信息
    }
    else if (pid2 == 0)
    {                                                            /* 子进程 */
        if (lock_reg(fd, F_SETLK, F_RDLCK, 0, SEEK_SET, 0) < 0)  // 尝试获取文件读锁
            err_sys("child 2: can't read-lock file");            // 如果获取锁失败,输出错误信息
        printf("child 2: obtained read lock on file\n");         // 成功获取锁后输出信息
        pause();                                                 // 暂停子进程,等待信号
        printf("child 2: exit after pause\n");                   // 暂停结束后输出信息
        exit(0);                                                 // 子进程退出
    }
    else
    {              /* 父进程 */
        sleep(2);  // 父进程休眠2秒,等待子进程操作
    }

    /*
     * 父进程继续...尝试获取写锁的阻塞块
     */
    if ((pid3 = fork()) < 0)  // 创建子进程
    {
        err_sys("fork failed");  // 如果fork失败,打印错误信息
    }
    else if (pid3 == 0)
    {                                                                        /* 子进程 */
        if (lock_reg(fd, F_SETLK, F_WRLCK, 0, SEEK_SET, 0) < 0)              // 尝试非阻塞地获取写锁
            printf("child 3: can't set write lock: %s\n", strerror(errno));  // 获取失败,打印错误信息
        printf("child 3 about to block in write-lock...\n");                 // 准备阻塞等待写锁
        if (lock_reg(fd, F_SETLKW, F_WRLCK, 0, SEEK_SET, 0) < 0)             // 阻塞地获取写锁
            err_sys("child 3: can't write-lock file");                       // 获取失败,打印错误信息
        printf("child 3 returned and got write lock????\n");                 // 成功获取写锁
        pause();                                                             // 暂停进程,等待信号
        printf("child 3: exit after pause\n");                               // 暂停后退出
        exit(0);
    }
    else
    {              /* 父进程 */
        sleep(2);  // 父进程休眠2秒
    }

    /*
     * 检查挂起的写锁是否会阻塞下一次读锁尝试.
     */
    // 尝试设置读锁,如果返回值小于0,表示失败
    if (lock_reg(fd, F_SETLK, F_RDLCK, 0, SEEK_SET, 0) < 0)
        // 打印错误信息
        printf("parent: can't set read lock: %s\n", strerror(errno));
    else
        // 如果成功获取读锁,打印成功信息
        printf("parent: obtained additional read lock while write lock is pending\n");
    // 发送信号终止子进程1
    printf("killing child 1...\n");
    kill(pid1, SIGINT);
    // 发送信号终止子进程2
    printf("killing child 2...\n");
    kill(pid2, SIGINT);
    // 发送信号终止子进程3
    printf("killing child 3...\n");
    kill(pid3, SIGINT);
    // 退出父进程
    exit(0);
}
