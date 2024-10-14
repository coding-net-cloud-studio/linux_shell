#include <sys/wait.h>
#include <errno.h>
#include <unistd.h>

/**
 * @brief 执行一个shell命令
 *
 * @param cmdstring 要执行的shell命令字符串
 * @return int 命令执行的状态码,如果出错返回-1
 */
int system(const char *cmdstring) /* version without signal handling */
{
    pid_t pid;     // 进程ID
    int   status;  // 子进程的退出状态

    // 如果命令字符串为空,直接返回错误码
    if (cmdstring == NULL)
        return (1); /* always a command processor with UNIX */

    // 创建子进程
    if ((pid = fork()) < 0)
    {
        status = -1;  // 创建子进程失败
    }
    else if (pid == 0)
    { /* child */
        // 在子进程中执行shell命令
        execl("/bin/sh", "sh", "-c", cmdstring, (char *)0);
        _exit(127);  // 如果execl执行失败,退出子进程
    }
    else
    { /* parent */
        // 等待子进程结束
        while (waitpid(pid, &status, 0) < 0)
        {
            // 如果不是因为EINTR中断,说明waitpid出错
            if (errno != EINTR)
            {
                status = -1;  // 设置状态为错误
                break;
            }
        }
    }

    return (status);  // 返回子进程的退出状态
}
