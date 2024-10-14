#include "apue.h"
#include <sys/wait.h>

// <summary>
// 打印进程退出状态信息.
// </summary>
// <param name="status">子进程的退出状态码.</param>
void pr_exit(int status)
{
    // 如果进程正常退出,则打印退出状态码
    // 检查进程是否正常退出,如果是,则打印退出状态码
    if (WIFEXITED(status))
        printf("normal termination, exit status = %d\n", WEXITSTATUS(status));
    // 如果进程异常终止,则打印终止进程的信号编号
    // WTERMSIG(status) 返回导致子进程终止的信号编号
    else if (WIFSIGNALED(status))
        printf("abnormal termination, signal number = %d%s\n", WTERMSIG(status),

#ifdef WCOREDUMP
               // 如果生成了核心转储文件,则附加相应的信息
               WCOREDUMP(status) ? " (core file generated)" : "");
#else
               "");
#endif
    // 如果进程被停止,则打印停止进程的信号编号
    // 如果子进程被停止,获取并打印停止信号编号
    else if (WIFSTOPPED(status))
        // WSTOPSIG宏用于获取导致子进程停止的信号编号
        printf("子进程已停止,信号编号 = %d\n", WSTOPSIG(status));
}
