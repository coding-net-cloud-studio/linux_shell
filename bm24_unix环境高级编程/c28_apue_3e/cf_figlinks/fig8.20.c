#include "apue.h"      // 引入apue库,提供系统编程接口
#include <sys/wait.h>  // 引入wait系统调用头文件,用于等待子进程结束

// 主函数
int main(void)
{
    pid_t pid;  // 定义进程ID变量

    // 使用fork创建子进程
    if ((pid = fork()) < 0)
    {
        err_sys("fork error");  // 如果fork失败,打印错误信息并退出
    }
    else if (pid == 0)
    {  // 子进程执行的代码块
       // 使用execl替换当前进程的镜像为指定程序
       // 第一个参数是可执行文件的路径,后续参数是传递给该程序的命令行参数
        // execl函数用于替换当前进程的镜像,用一个新的进程镜像来执行.
        // 参数分别是:要执行的程序文件的路径,程序名(通常与文件名相同),以及传递给程序的参数列表,参数列表以NULL结束.
        // 如果函数执行成功,当前进程将被新进程替换,不会返回;如果失败,则返回-1,并设置errno以指示错误.
        if (execl("/home/sar/bin/testinterp",  // 要执行的程序文件路径
                  "testinterp",                // 程序名
                  "myarg1",                    // 第一个参数
                  "MY ARG2",                   // 第二个参数
                  (char *)0) < 0)              // 参数列表结束标志
                                               // 如果execl函数返回值小于0,表示执行失败,这里可以添加错误处理代码
            err_sys("execl error");            // 如果execl失败,打印错误信息
    }
    // 父进程等待子进程结束
    if (waitpid(pid, NULL, 0) < 0)
        err_sys("waitpid error");  // 如果waitpid失败,打印错误信息
    exit(0);                       // 正常退出程序
}
