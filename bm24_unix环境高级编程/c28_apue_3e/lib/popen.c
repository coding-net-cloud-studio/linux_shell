#include "apue.h"      // 引入自定义的头文件,可能包含程序中使用的各种函数和宏定义
#include <errno.h>     // 引入错误处理头文件,定义了各种错误代码
#include <fcntl.h>     // 引入文件控制操作头文件,如open, fcntl等函数
#include <sys/wait.h>  // 引入系统等待子进程的头文件,包含wait和waitpid函数

/*
 * Pointer to array allocated at run-time.  // 运行时分配的数组指针
 */
static pid_t *childpid = NULL;  // 定义一个静态变量,用于存储子进程的PID

/*
 * From our open_max(), {Prog openmax}.  // 来自我们的open_max()函数,{Prog openmax}可能是文档的一部分或者是注释者的说明
 */
static int maxfd;  // 定义一个静态变量,用于存储系统中最大的文件描述符
/*
 * popen - 打开一个指向命令的管道
 * @cmdstring: 要执行的命令字符串
 * @type: 文件描述符类型,"r"表示读取,"w"表示写入
 * 返回: 成功返回文件指针,失败返回NULL
 */
FILE *
popen(const char *cmdstring, const char *type)
{
    int   i;
    int   pfd[2];  // 管道文件描述符数组
    pid_t pid;     // 进程ID
    FILE *fp;      // 文件指针

    /* 只允许 "r" 或 "w" */
    if ((type[0] != 'r' && type[0] != 'w') || type[1] != 0)
    {
        errno = EINVAL;  // 设置无效参数错误
        return (NULL);
    }

    // 如果childpid为NULL,表示这是第一次执行
    if (childpid == NULL)
    {
        /* 为子进程PID分配一个全零数组 */
        // 获取系统允许的最大文件描述符数
        maxfd = open_max();
        // 分配内存空间给childpid数组
        if ((childpid = calloc(maxfd, sizeof(pid_t))) == NULL)
            return (NULL);  // 如果分配失败,返回NULL
    }

    // 创建一个管道
    if (pipe(pfd) < 0)
        return (NULL); /* errno由pipe()设置,表示创建管道失败 */

    // 检查管道的文件描述符是否超过了系统限制
    // 检查管道文件描述符是否超出最大文件描述符数
    if (pfd[0] >= maxfd || pfd[1] >= maxfd)
    {
        // 关闭管道的两端,释放资源
        close(pfd[0]);
        close(pfd[1]);
        errno = EMFILE;  // 设置错误码为EMFILE,表示文件描述符过多
        return (NULL);   // 返回NULL,表示操作失败
    }

    if ((pid = fork()) < 0)  // 创建子进程
    {
        return (NULL); /* errno由fork()设置 */
    }
    else if (pid == 0)
    { /* 子进程 */
        // 如果type为'r',表示需要从子进程读取数据
        if (*type == 'r')
        {
            close(pfd[0]);  // 关闭读端,因为不需要从父进程读取数据
            // 如果写端不是标准输出,则将写端复制到标准输出
            if (pfd[1] != STDOUT_FILENO)
            {
                dup2(pfd[1], STDOUT_FILENO);  // 将写端复制到标准输出
                close(pfd[1]);                // 关闭原写端文件描述符
            }
        }
        else
        {
            close(pfd[1]);  // 关闭写端,因为不需要向子进程写入数据
            // 如果读端不是标准输入,则将读端复制到标准输入
            if (pfd[0] != STDIN_FILENO)
            {
                dup2(pfd[0], STDIN_FILENO);  // 将读端复制到标准输入
                close(pfd[0]);               // 关闭原读端文件描述符
            }
        }

        /* 关闭子进程pid数组中的所有描述符 */
        for (i = 0; i < maxfd; i++)
            if (childpid[i] > 0)
                close(i);

        execl("/bin/sh", "sh", "-c", cmdstring, (char *)0);  // 执行命令
        _exit(127);                                          // 执行失败退出
    }

    /* 父进程继续... */
    if (*type == 'r')
    {
        close(pfd[1]);                            // 关闭写端
        if ((fp = fdopen(pfd[0], type)) == NULL)  // 将读端转换为文件指针
            return (NULL);
    }
    else
    {
        close(pfd[0]);                            // 关闭读端
        if ((fp = fdopen(pfd[1], type)) == NULL)  // 将写端转换为文件指针
            return (NULL);
    }

    childpid[fileno(fp)] = pid;  // 记录此文件描述符对应的子进程PID
    return (fp);                 // 返回文件指针
}

/*
 * pclose - 关闭由popen()打开的进程并等待其结束
 *
 * @fp: 文件指针,由popen()返回
 *
 * 返回值: 子进程的退出状态,如果出错则返回-1
 */
int pclose(FILE *fp)
{
    int   fd, stat;  // fd为文件描述符,stat为子进程的退出状态
    pid_t pid;       // pid为子进程的进程ID

    // 如果childpid为NULL,说明popen()未被调用过
    // 检查子进程ID是否为空
    if (childpid == NULL)
    {
        // 如果是空的,设置errno为EINVAL,表示无效参数
        errno = EINVAL;
        // 返回-1,表示函数执行失败
        return (-1);
    }
    // 注释结束

    // 获取文件描述符
    fd = fileno(fp);
    // 如果文件描述符无效
    // 检查文件描述符是否超出最大值
    if (fd >= maxfd)
    {
        errno = EINVAL;  // 设置错误码为无效参数
        return (-1);     // 返回-1表示失败
    }

    // 如果fp不是由popen()打开的
    if ((pid = childpid[fd]) == 0)
    {
        errno = EINVAL;  // 设置错误码为无效参数
        return (-1);     // 返回-1表示失败
    }

    // 标记该文件描述符已关闭
    childpid[fd] = 0;
    // 关闭文件指针
    if (fclose(fp) == EOF)
        return (-1);

    // 等待子进程结束
    while (waitpid(pid, &stat, 0) < 0)
        // 如果不是因为EINTR错误,则返回-1
        if (errno != EINTR)
            return (-1);

    // 返回子进程的退出状态
    return (stat);
}
