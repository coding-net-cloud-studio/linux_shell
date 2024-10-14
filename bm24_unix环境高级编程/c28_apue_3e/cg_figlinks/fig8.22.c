#include <sys/wait.h>  // 包含wait系统调用的头文件,用于等待子进程结束
#include <errno.h>     // 包含错误码定义的头文件,用于错误处理
#include <unistd.h>    // 包含unistd系统调用的头文件,如fork(), exec()等

// system 函数用于执行 shell 命令
// 参数 cmdstring 是要执行的命令字符串
// 返回值:命令成功执行返回 0,失败返回 -1 或命令的退出状态
int system(const char *cmdstring) /* version without signal handling */
{
    pid_t pid;     // 用于存储 fork 函数的返回值,即子进程的 PID
    int   status;  // 用于存储 waitpid 函数的返回状态

    if (cmdstring == NULL)
        return (1); /* always a command processor with UNIX */

    // 使用 fork 创建子进程
    if ((pid = fork()) < 0)
    {
        status = -1; /* probably out of processes */
    }
    else if (pid == 0)
    { /* child */
        // 子进程中执行 execl,替换当前进程的镜像为 /bin/sh,并执行 cmdstring 指定的命令
        execl("/bin/sh", "sh", "-c", cmdstring, (char *)0);
        // 如果 execl 执行失败,则退出子进程并返回错误码 127
        _exit(127); /* execl error */
    }
    else
    { /* parent */
        // 父进程中等待子进程结束
        while (waitpid(pid, &status, 0) < 0)
        {
            // 如果 waitpid 返回错误且错误码不是 EINTR,则退出循环
            // 如果errno不等于EINTR,表示waitpid()函数出错,且错误不是由于信号中断引起的
            if (errno != EINTR)
            {
                status = -1;  // 设置状态为-1,表示出现了除EINTR之外的错误
                break;        // 跳出循环
            }
        }
    }

    return (status);  // 返回命令执行的状态
}
