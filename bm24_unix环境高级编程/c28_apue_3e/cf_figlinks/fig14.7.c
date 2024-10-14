#include "apue.h"
#include <fcntl.h>

// 锁定指定文件偏移量上的一个字节
// @param name 文件名或描述
// @param fd 文件描述符
// @param offset 要锁定的字节偏移量
static void
lockabyte(const char *name, int fd, off_t offset)
{
    // 尝试获取写锁,如果失败则打印错误信息并退出
    if (writew_lock(fd, offset, SEEK_SET, 1) < 0)
        err_sys("%s: writew_lock error", name);
    // 成功获取锁后,打印锁定信息
    printf("%s: got the lock, byte %lld\n", name, (long long)offset);
}

int main(void)
{
    int   fd;   // 文件描述符
    pid_t pid;  // 进程ID

    /*
     * 创建一个文件并向其中写入两个字节.
     */
    if ((fd = creat("templock", FILE_MODE)) < 0)  // 创建文件,FILE_MODE为文件权限模式
        err_sys("creat error");                   // 如果创建失败,调用err_sys打印错误信息
    if (write(fd, "ab", 2) != 2)                  // 向文件写入"ab"两个字节
        err_sys("write error");                   // 如果写入失败,调用err_sys打印错误信息

    TELL_WAIT();             // 通知子进程等待父进程的信号
    if ((pid = fork()) < 0)  // 创建子进程
    {
        err_sys("fork error");  // 如果创建失败,调用err_sys打印错误信息
    }
    else if (pid == 0)
    {                               /* child */
        lockabyte("child", fd, 0);  // 子进程锁定文件的第0个字节
        TELL_PARENT(getppid());     // 通知父进程可以继续执行
        WAIT_PARENT();              // 等待父进程的信号
        lockabyte("child", fd, 1);  // 子进程锁定文件的第1个字节
    }
    else
    {                                /* parent */
        lockabyte("parent", fd, 1);  // 父进程锁定文件的第1个字节
        TELL_CHILD(pid);             // 通知子进程可以继续执行
        WAIT_CHILD();                // 等待子进程的信号
        lockabyte("parent", fd, 0);  // 父进程锁定文件的第0个字节
    }
    exit(0);  // 退出程序
}
