#include "apue.h"      // 包含APUE库的头文件,提供系统编程接口
#include <sys/wait.h>  // 包含wait系统调用的头文件,用于等待子进程结束

// <summary>
// 打印进程退出状态信息.
// </summary>
// <param name="status">进程退出状态码.</param>
void pr_exit(int status)
{
    // 如果进程正常终止,打印退出状态码
    // 检查进程是否正常退出
    if (WIFEXITED(status))
        // 如果进程正常退出,打印退出状态码
        printf("normal termination, exit status = %d\n", WEXITSTATUS(status));
    // 如果进程异常终止,打印终止进程的信号编号
    else if (WIFSIGNALED(status))
        // 打印异常终止的信号编号
        printf("abnormal termination, signal number = %d%s\n", WTERMSIG(status),
#ifdef WCOREDUMP
               // 如果生成了核心转储文件,则附加相应的信息
               WCOREDUMP(status) ? " (core file generated)" : "");
#else
               "");
#endif

    // 如果子进程被停止,打印导致停止的信号编号
    // 如果子进程被停止,获取并打印停止信号编号
    else if (WIFSTOPPED(status))
    {
        // WSTOPSIG(status) 返回停止子进程的信号编号
        printf("子进程被停止,信号编号 = %d\n", WSTOPSIG(status));
    }
}
