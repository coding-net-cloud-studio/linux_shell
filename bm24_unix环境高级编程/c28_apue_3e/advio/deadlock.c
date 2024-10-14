#include "apue.h"
#include <fcntl.h>

// 锁定指定文件偏移量处的字节
// @param name 锁定操作的名称,用于日志输出
// @param fd 文件描述符
// @param offset 要锁定的字节偏移量
static void
lockabyte(const char *name, int fd, off_t offset)
{
    // 尝试获取写锁,如果失败则输出错误信息并退出
    if (writew_lock(fd, offset, SEEK_SET, 1) < 0)
        err_sys("%s: writew_lock error", name);
    // 成功获取锁后,输出锁定信息
    printf("%s: got the lock, byte %lld\n", name, (long long)offset);
}

int main(void)
{
    int   fd;   // 文件描述符
    pid_t pid;  // 进程ID

    // 创建一个文件并写入两个字节
    if ((fd = creat("templock", FILE_MODE)) < 0)
        err_sys("creat error");
    if (write(fd, "ab", 2) != 2)
        err_sys("write error");

    // 初始化父子进程间通信
    TELL_WAIT();
    // 创建子进程
    if ((pid = fork()) < 0)
    {
        // fork失败,输出错误信息并退出
        err_sys("fork error");
    }
    else if (pid == 0)
    { /* child */
        // 子进程锁定偏移量为0的字节
        lockabyte("child", fd, 0);
        // 通知父进程已锁定
        TELL_PARENT(getppid());
        // 等待父进程解锁
        WAIT_PARENT();
        // 子进程尝试锁定偏移量为1的字节
        lockabyte("child", fd, 1);
    }
    else
    { /* parent */
        // 父进程锁定偏移量为1的字节
        lockabyte("parent", fd, 1);
        // 通知子进程已锁定
        TELL_CHILD(pid);
        // 等待子进程解锁
        WAIT_CHILD();
        // 父进程尝试锁定偏移量为0的字节
        lockabyte("parent", fd, 0);
    }
    // 退出进程
    exit(0);
}
