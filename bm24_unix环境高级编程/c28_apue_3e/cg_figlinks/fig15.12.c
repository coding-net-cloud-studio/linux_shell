#include "apue.h"      // 引入自定义的应用程序环境头文件
#include <errno.h>     // 引入错误处理头文件,定义了errno变量和各种错误代码
#include <fcntl.h>     // 引入文件控制操作头文件,如open, fcntl等函数
#include <sys/wait.h>  // 引入系统等待子进程状态改变的头文件,包含wait和waitpid函数

/*
 * 指向运行时分配的数组的指针.
 * 这个指针用于存储子进程的PID,每个元素对应一个子进程.
 */
static pid_t *childpid = NULL;

/*
 * 来自我们的open_max()函数,{Prog openmax}.
 * maxfd变量用于存储当前进程可以打开的最大文件描述符数.
 */
static int maxfd;

/*
 * popen 函数用于执行一个命令并创建一个与命令的输入/输出流相关联的文件指针.
 * @param cmdstring 要执行的命令字符串
 * @param type 指定文件指针的类型,"r" 表示读取命令的输出,"w" 表示写入命令的输入
 * @return 成功时返回一个文件指针,失败时返回NULL,并设置errno以指示错误
 */
FILE *
popen(const char *cmdstring, const char *type)

{

    // 声明整型变量i,用于循环或其他计数目的
    int i;

    // 声明一个包含两个元素的整型数组pfd,通常用于存储管道的文件描述符
    int pfd[2];

    // 声明pid_t类型的变量pid,用于存储进程ID
    pid_t pid;

    // 声明一个FILE指针fp,用于文件操作
    FILE *fp;

    // 仅允许 "r" 或 "w" 模式
    // 如果 type 不是以 'r' 或 'w' 开头,或者 type 的第二个字符不是字符串结束符 '\0',
    // 则设置 errno 为 EINVAL 并返回 NULL.
    if ((type[0] != 'r' && type[0] != 'w') || type[1] != 0)
    {
        errno = EINVAL;
        return (NULL);
    }
    // 如果childpid为NULL,表示这是第一次执行
    if (childpid == NULL)
    {
        // 分配一个全为零的数组来存储子进程的pid
        // open_max()函数用于获取系统中可以打开的最大文件描述符数
        maxfd = open_max();
        // 使用calloc分配内存,大小为maxfd乘以pid_t类型的大小
        // 如果分配失败,返回NULL
        if ((childpid = calloc(maxfd, sizeof(pid_t))) == NULL)
            return (NULL);
    }

    // 创建一个管道,pfd是一个包含两个文件描述符的数组,pfd[0]用于读取,pfd[1]用于写入
    if (pipe(pfd) < 0)
        return (NULL);  // 如果pipe()函数失败,返回NULL,并且errno会被设置为相应的错误代码

    // 检查文件描述符是否超过了当前进程可以打开的最大文件数
    if (pfd[0] >= maxfd || pfd[1] >= maxfd)
    {
        close(pfd[0]);   // 关闭读端文件描述符
        close(pfd[1]);   // 关闭写端文件描述符
        errno = EMFILE;  // 设置errno为EMFILE,表示进程已达到可打开文件数的上限
        return (NULL);   // 返回NULL,表示创建管道失败
    }

    // 使用fork创建子进程
    if ((pid = fork()) < 0)
    {
        return (NULL);  // 如果fork失败,返回NULL,errno已由fork设置
    }
    else if (pid == 0)
    {  // 子进程
        // 如果type是'r',重定向标准输出到pfd[1]
        if (*type == 'r')
        {
            close(pfd[0]);                // 关闭pfd[0],因为它不再需要了
            if (pfd[1] != STDOUT_FILENO)  // 如果pfd[1]不是标准输出
            {
                dup2(pfd[1], STDOUT_FILENO);  // 将pfd[1]复制到标准输出
                close(pfd[1]);                // 关闭pfd[1]的原始副本
            }
        }
        else
        {
            // 如果type不是'r',重定向标准输入到pfd[0]
            close(pfd[1]);               // 关闭pfd[1],因为它不再需要了
            if (pfd[0] != STDIN_FILENO)  // 如果pfd[0]不是标准输入
            {
                dup2(pfd[0], STDIN_FILENO);  // 将pfd[0]复制到标准输入
                close(pfd[0]);               // 关闭pfd[0]的原始副本
            }
        }

        // 关闭子进程中的所有描述符
        for (i = 0; i < maxfd; i++)
        {
            if (childpid[i] > 0)  // 如果childpid[i]大于0,表示该描述符是打开的
            {
                close(i);  // 关闭描述符
            }
        }

        // 执行新的shell命令
        execl("/bin/sh", "sh", "-c", cmdstring, (char *)0);  // 使用execl函数替换当前进程映像,执行sh命令
        _exit(127);                                          // 如果execl失败,退出进程并返回错误代码127
    }

    /* parent继续执行... */
    // 如果type是'r',关闭pfd[1],然后尝试使用fdopen函数以type模式打开pfd[0]对应的文件描述符
    if (*type == 'r')
    {
        close(pfd[1]);                            // 关闭读端文件描述符
        if ((fp = fdopen(pfd[0], type)) == NULL)  // 尝试打开文件
            return (NULL);                        // 如果打开失败,返回NULL
    }
    else
    {
        close(pfd[0]);                            // 如果type不是'r',关闭pfd[0],即写端文件描述符
        if ((fp = fdopen(pfd[1], type)) == NULL)  // 尝试打开文件
            return (NULL);                        // 如果打开失败,返回NULL
    }

    // 为文件描述符对应的子进程PID赋值
    // 这样做是为了记住每个文件描述符对应的子进程的PID,便于后续管理
    childpid[fileno(fp)] = pid;
    // 返回文件指针,以便后续操作
    return (fp);
}

/**
 * 关闭与指定文件流关联的管道,并等待子进程结束.
 *
 * @param fp 文件流指针,指向要关闭的管道文件.
 * @return 如果成功关闭管道并获取子进程状态,则返回子进程的退出状态;如果发生错误,则返回-1,并设置errno.
 */
int pclose(FILE *fp)
{

    // 定义文件描述符变量fd,用于存储打开文件的描述符
    int fd;

    // 定义状态变量stat,可能用于存储文件状态或其他操作结果
    int stat;

    // 定义进程ID变量pid,用于存储创建子进程后的进程ID
    pid_t pid;

    // 检查 childpid 数组是否为空,如果为空则表示 popen() 函数从未被调用过
    if (childpid == NULL)
    {
        // 设置错误号为 EINVAL,表示参数无效
        errno = EINVAL;
        // 返回-1,表示函数执行失败
        return (-1);
    }

    // 获取文件流的文件描述符
    fd = fileno(fp);
    // 检查文件描述符是否超出了允许的最大值
    if (fd >= maxfd)
    {
        // 设置错误号为 EINVAL,表示文件描述符无效
        errno = EINVAL;
        // 返回-1,表示函数执行失败
        return (-1);
    }
    // 检查 childpid 数组中对应文件描述符的位置是否为0,如果为0则表示该文件流不是由 popen() 函数打开的
    if ((pid = childpid[fd]) == 0)
    {
        // 设置错误号为 EINVAL,表示文件描述符无效
        errno = EINVAL;
        // 返回-1,表示函数执行失败
        return (-1);
    }

    // 将 childpid 数组中对应文件描述符的位置清零,表示该文件流已经被关闭
    childpid[fd] = 0;
    // 关闭文件流
    if (fclose(fp) == EOF)
        return (-1);

    // 等待子进程结束,如果 waitpid() 函数返回错误且错误号不是 EINTR,则返回-1
    // 等待子进程结束
    // 使用waitpid函数等待指定pid的子进程结束,stat用于存储子进程的退出状态
    // 如果waitpid返回值小于0,表示出错
    while (waitpid(pid, &stat, 0) < 0)
        // 如果错误不是因为被信号中断(EINTR),则返回-1表示失败
        if (errno != EINTR)
            return (-1);

    // 返回子进程的退出状态
    return (stat);
}
