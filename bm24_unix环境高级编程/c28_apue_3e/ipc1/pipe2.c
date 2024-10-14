#include "apue.h"
#include <sys/wait.h>

#define DEF_PAGER "/bin/more" /* 默认的分页程序 */

int main(int argc, char *argv[])
{
    int   n;              // 用于存储读取的行长度
    int   fd[2];          // 管道文件描述符数组
    pid_t pid;            // 进程ID
    char *pager, *argv0;  // pager程序的路径和参数
    char  line[MAXLINE];  // 存储从文件读取的行
    FILE *fp;             // 文件指针

    if (argc != 2)                            // 检查命令行参数数量
        err_quit("usage: a.out <pathname>");  // 参数数量不正确,退出程序

    if ((fp = fopen(argv[1], "r")) == NULL)  // 打开文件
        err_sys("can't open %s", argv[1]);   // 文件打开失败,退出程序
    if (pipe(fd) < 0)                        // 创建管道
        err_sys("pipe error");               // 管道创建失败,退出程序

    if ((pid = fork()) < 0)  // 创建子进程
    {
        err_sys("fork error");  // 子进程创建失败,退出程序
    }
    else if (pid > 0)  // 父进程
    {
        close(fd[0]);  // 关闭管道的读端

        /* 父进程将argv[1]的内容复制到管道 */
        while (fgets(line, MAXLINE, fp) != NULL)  // 读取文件的每一行
        {
            n = strlen(line);                    // 获取行长度
            if (write(fd[1], line, n) != n)      // 将行写入管道
                err_sys("write error to pipe");  // 写入失败,退出程序
        }
        if (ferror(fp))  // 检查文件读取错误
            err_sys("fgets error");

        close(fd[1]);  // 关闭管道的写端

        if (waitpid(pid, NULL, 0) < 0)  // 等待子进程结束
            err_sys("waitpid error");   // 等待失败,退出程序
        exit(0);
    }
    else  // 子进程
    {
        close(fd[1]);               // 关闭管道的写端
        if (fd[0] != STDIN_FILENO)  // 如果管道读端不是标准输入
        {
            if (dup2(fd[0], STDIN_FILENO) != STDIN_FILENO)  // 将管道读端复制到标准输入
                err_sys("dup2 error to stdin");             // 复制失败,退出程序
            close(fd[0]);                                   // 关闭原管道读端
        }

        /* 获取execl()的参数 */
        if ((pager = getenv("PAGER")) == NULL)      // 获取环境变量PAGER
            pager = DEF_PAGER;                      // 如果没有设置,则使用默认分页程序
        if ((argv0 = strrchr(pager, '/')) != NULL)  // 查找pager路径中的最后一个斜杠
            argv0++;                                // 移动到斜杠之后
        else
            argv0 = pager;  // 如果没有斜杠,argv0就是pager

        if (execl(pager, argv0, (char *)0) < 0)    // 执行分页程序
            err_sys("execl error for %s", pager);  // 执行失败,退出程序
    }
    exit(0);
}
