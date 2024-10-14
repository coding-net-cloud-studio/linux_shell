#include "apue.h"   // 引入APUE库头文件
#include <fcntl.h>  // 引入文件控制相关的头文件
#include <errno.h>  // 引入错误处理相关的头文件

// @brief 信号处理函数,用于处理中断信号
// @param signo 接收到的信号编号
void sigint(int signo)
{
    // 该函数目前为空,可能需要根据实际需求添加处理逻辑
}

int main(void)
{
    pid_t pid1, pid2, pid3;  // 定义三个进程ID变量
    int   fd;                // 文件描述符

    setbuf(stdout, NULL);         // 设置标准输出不进行缓冲
    signal_intr(SIGINT, sigint);  // 设置SIGINT信号的处理函数为sigint

    /*
     * 创建一个文件.
     */
    if ((fd = open("lockfile", O_RDWR | O_CREAT, 0666)) < 0)  // 打开或创建文件,并设置权限
        err_sys("can't open/create lockfile");                // 如果打开或创建失败,打印错误信息并退出

    /*
     * 对文件进行读锁.
     */
    if ((pid1 = fork()) < 0)  // 创建第一个子进程
    {
        err_sys("fork failed");  // 如果创建子进程失败,打印错误信息并退出
    }
    else if (pid1 == 0)
    {                                                            /* 子进程 */
        if (lock_reg(fd, F_SETLK, F_RDLCK, 0, SEEK_SET, 0) < 0)  // 尝试获取读锁
            err_sys("child 1: can't read-lock file");            // 如果获取读锁失败,打印错误信息并退出
        printf("child 1: obtained read lock on file\n");         // 获取读锁成功,打印信息
        pause();                                                 // 暂停进程,等待信号
        printf("child 1: exit after pause\n");                   // 收到信号后,打印退出信息
        exit(0);                                                 // 退出子进程
    }
    else
    {              /* 父进程 */
        sleep(2);  // 父进程等待2秒
    }

    /*
     * 父进程继续...再次对文件进行读锁.
     */
    if ((pid2 = fork()) < 0)  // 创建第二个子进程
    {
        err_sys("fork failed");  // 如果创建子进程失败,打印错误信息并退出
    }
    else if (pid2 == 0)
    {                                                            /* 子进程 */
        if (lock_reg(fd, F_SETLK, F_RDLCK, 0, SEEK_SET, 0) < 0)  // 尝试获取读锁
            err_sys("child 2: can't read-lock file");            // 如果获取读锁失败,打印错误信息并退出
        printf("child 2: obtained read lock on file\n");         // 获取读锁成功,打印信息
        pause();                                                 // 暂停进程,等待信号
        printf("child 2: exit after pause\n");                   // 收到信号后,打印退出信息
        exit(0);                                                 // 退出子进程
    }
    else
    {              /* 父进程 */
        sleep(2);  // 父进程等待2秒
    }

    /*
     * 父进程继续...尝试对文件进行写锁,此时应该会阻塞.
     */
    if ((pid3 = fork()) < 0)  // 创建第三个子进程
    {
        err_sys("fork failed");  // 如果创建子进程失败,打印错误信息并退出
    }
    else if (pid3 == 0)
    {                                                                        /* 子进程 */
        if (lock_reg(fd, F_SETLK, F_WRLCK, 0, SEEK_SET, 0) < 0)              // 尝试获取写锁(非阻塞)
            printf("child 3: can't set write lock: %s\n", strerror(errno));  // 获取写锁失败,打印错误信息
        printf("child 3 about to block in write-lock...\n");                 // 打印即将阻塞的信息
        if (lock_reg(fd, F_SETLKW, F_WRLCK, 0, SEEK_SET, 0) < 0)             // 尝试获取写锁(阻塞)
            err_sys("child 3: can't write-lock file");                       // 如果获取写锁失败,打印错误信息并退出
        printf("child 3 returned and got write lock????\n");                 // 获取写锁成功,打印信息
        pause();                                                             // 暂停进程,等待信号
        printf("child 3: exit after pause\n");                               // 收到信号后,打印退出信息
        exit(0);                                                             // 退出子进程
    }
    else
    {              /* 父进程 */
        sleep(2);  // 父进程等待2秒
    }

    /*
     * 查看挂起的写锁是否会阻塞下一个读锁尝试.
     */
    if (lock_reg(fd, F_SETLK, F_RDLCK, 0, SEEK_SET, 0) < 0)            // 尝试获取读锁
        printf("parent: can't set read lock: %s\n", strerror(errno));  // 如果获取读锁失败,打印错误信息
    else
        printf("parent: obtained additional read lock while write lock is pending\n");  // 获取读锁成功,打印信息
    printf("killing child 1...\n");                                                     // 打印即将杀死第一个子进程的信息
    kill(pid1, SIGINT);                                                                 // 发送SIGINT信号给第一个子进程
    printf("killing child 2...\n");                                                     // 打印即将杀死第二个子进程的信息
    kill(pid2, SIGINT);                                                                 // 发送SIGINT信号给第二个子进程
    printf("killing child 3...\n");                                                     // 打印即将杀死第三个子进程的信息
    kill(pid3, SIGINT);                                                                 // 发送SIGINT信号给第三个子进程
    exit(0);                                                                            // 退出父进程
}
