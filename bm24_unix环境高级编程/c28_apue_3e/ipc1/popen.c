#include "apue.h"      // 引入apue库的头文件
#include <errno.h>     // 引入错误处理库的头文件
#include <fcntl.h>     // 引入文件控制库的头文件
#include <sys/wait.h>  // 引入系统等待子进程结束的头文件

/*
 * 指向运行时分配的数组的指针.
 */
static pid_t *childpid = NULL;

/*
 * 来自我们的 open_max(),{Prog openmax}.
 */
static int maxfd;

/**
 * @brief 执行一个命令并创建一个与之关联的进程,允许读取或写入命令的标准输入/输出.
 *
 * @param cmdstring 要执行的命令字符串.
 * @param type 文件描述符类型,"r" 表示读取,"w" 表示写入.
 * @return FILE* 成功时返回文件指针,失败时返回 NULL.
 */
FILE *
popen(const char *cmdstring, const char *type)
{
    // 变量声明
    int   i;       // 循环变量
    int   pfd[2];  // 管道文件描述符数组,pfd[0]用于读取,pfd[1]用于写入
    pid_t pid;     // 进程ID
    FILE *fp;      // 文件指针,用于操作管道

    // 注释说明:
    // 变量i将用于循环控制,在后续代码中可能会有循环操作.
    // pfd数组用于创建管道,实现进程间通信.
    // pid用于存储fork()函数创建子进程后的返回值,以区分父进程和子进程.
    // fp是一个文件指针,它将被用于打开和操作管道文件.

    /* 只允许 "r" 或 "w" */
    if ((type[0] != 'r' && type[0] != 'w') || type[1] != 0)
    {
        errno = EINVAL;
        return (NULL);
    }

    if (childpid == NULL)
    { /* 第一次调用 */
        /* 为子进程 pid 分配零初始化的数组 */
        // 获取系统中可以打开的最大文件描述符数
        maxfd = open_max();

        // 分配内存空间,用于存储每个文件描述符对应的子进程PID
        // 如果分配失败,返回NULL
        if ((childpid = calloc(maxfd, sizeof(pid_t))) == NULL)
            return (NULL);
    }

    if (pipe(pfd) < 0)
        return (NULL); /* pipe() 设置了 errno */
    // 检查文件描述符数组pfd中的文件描述符是否超过了当前进程的最大文件数maxfd
    if (pfd[0] >= maxfd || pfd[1] >= maxfd)
    {
        // 如果超过,则关闭数组中的两个文件描述符
        close(pfd[0]);
        close(pfd[1]);
        // 设置errno为EMFILE,表示达到文件数上限的错误
        errno = EMFILE;
        // 返回NULL,表示操作失败
        return (NULL);
    }

    if ((pid = fork()) < 0)
    {
        return (NULL); /* fork() 设置了 errno */
    }
    else if (pid == 0)
    { /* 子进程 */
        // 如果type为'r',则关闭pfd[0],并将pfd[1]的内容复制到标准输出(STDOUT_FILENO),
        // 然后关闭pfd[1].如果不是,则关闭pfd[1],并将pfd[0]的内容复制到标准输入(STDIN_FILENO),
        // 然后关闭pfd[0].
        if (*type == 'r')
        {
            close(pfd[0]);
            if (pfd[1] != STDOUT_FILENO)
            {
                dup2(pfd[1], STDOUT_FILENO);  // 将pfd[1]的内容复制到标准输出
                close(pfd[1]);                // 关闭pfd[1]
            }
        }
        else
        {
            close(pfd[1]);
            if (pfd[0] != STDIN_FILENO)
            {
                dup2(pfd[0], STDIN_FILENO);  // 将pfd[0]的内容复制到标准输入
                close(pfd[0]);               // 关闭pfd[0]
            }
        }

        /* 关闭子进程pid数组中的所有描述符 */
        for (i = 0; i < maxfd; i++)
            if (childpid[i] > 0)  // 如果子进程pid数组中的描述符大于0,则关闭它
                close(i);

        execl("/bin/sh", "sh", "-c", cmdstring, (char *)0);  // 执行shell命令
        _exit(127);                                          // 如果execl失败,则退出进程,返回错误码127
    }

    /* 父进程继续... */
    // popen.c

    // 如果type是'r',关闭pfd[1],然后使用pfd[0]和type打开一个文件指针fp
    // 如果打开失败,返回NULL
    if (*type == 'r')
    {
        close(pfd[1]);                            // 关闭读写管道的写端
        if ((fp = fdopen(pfd[0], type)) == NULL)  // 使用文件描述符pfd[0]打开文件流
            return (NULL);                        // 打开失败,返回NULL
    }
    else
    {
        close(pfd[0]);                            // 关闭读写管道的读端
        if ((fp = fdopen(pfd[1], type)) == NULL)  // 使用文件描述符pfd[1]打开文件流
            return (NULL);                        // 打开失败,返回NULL
    }

    // 记录与此文件描述符相关联的子进程pid
    childpid[fileno(fp)] = pid;

    return (fp);  // 返回文件指针
}

/**
 * @brief 关闭由 popen() 打开的进程,并等待其结束.
 *
 * @param fp 由 popen() 返回的 FILE 指针.
 * @return int 成功时返回子进程的退出状态,失败时返回 -1.
 */
// pclose 函数用于关闭由 popen() 打开的进程,并等待该进程结束,返回子进程的退出状态.
// 参数:
//   fp: 由 popen() 打开的 FILE 指针.
//
// 返回值:
//   成功时返回子进程的退出状态,失败时返回 -1.
int pclose(FILE *fp)
{
    int   fd, stat;  // 定义文件描述符和子进程状态变量
    pid_t pid;       // 定义子进程 ID 变量

    // 检查 childpid 是否为 NULL,如果是,则表示 popen() 从未被调用
    if (childpid == NULL)
    {
        errno = EINVAL;
        return (-1);
    }

    // 获取 FILE 指针对应的文件描述符
    fd = fileno(fp);
    // 检查文件描述符是否有效
    if (fd >= maxfd)
    {
        errno = EINVAL;
        return (-1);
    }
    // 检查该文件描述符是否由 popen() 打开
    if ((pid = childpid[fd]) == 0)
    {
        errno = EINVAL;
        return (-1);
    }

    // 将 childpid 数组中对应的文件描述符位置置为 0
    childpid[fd] = 0;
    // 关闭 FILE 指针
    if (fclose(fp) == EOF)
        return (-1);

    // 等待子进程结束
    while (waitpid(pid, &stat, 0) < 0)
        // 如果 waitpid() 返回错误,但不是因为 EINTR,则返回 -1
        if (errno != EINTR)
            return (-1);

    // 返回子进程的退出状态
    return (stat);
}
