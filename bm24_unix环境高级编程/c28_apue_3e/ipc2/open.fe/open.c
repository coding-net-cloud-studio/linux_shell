#include "open.h"
#include <sys/uio.h> /* struct iovec */  // 引入sys/uio.h头文件,该头文件定义了用于散布/聚集I/O的结构体struct iovec

/*
 * Open the file by sending the "name" and "oflag" to the
 * connection server and reading a file descriptor back.
 */

/*
 * 打开文件,通过向连接服务器发送"name"和"oflag",
 * 然后读回一个文件描述符.
 *
 * @param name 文件名
 * @param oflag 打开标志
 * @return 成功返回文件描述符,失败返回-1
 */

// csopen 函数用于打开一个文件,根据提供的文件名和打开标志.
// 参数:
//   name - 要打开的文件的名称.
//   oflag - 打开文件时使用的标志,例如只读,写入或追加模式.
// 返回值:
//   成功时返回文件描述符,失败时返回错误代码.
int csopen(char *name, int oflag)

{
    // 定义进程ID变量
    pid_t pid;

    // 定义长度变量
    int len;

    // 定义字符数组,用于存储读取或写入的数据
    char buf[10];

    // 定义iovec结构体数组,用于scatter/gather I/O操作
    struct iovec iov[3];

    // 定义文件描述符数组,初始化为-1,表示尚未打开任何文件
    static int fd[2] = {-1, -1};

    if (fd[0] < 0)
    { /* fork/exec our open server first time */
        // 创建一个管道,fd[0]用于读取,fd[1]用于写入
        if (fd_pipe(fd) < 0)
        {
            err_ret("fd_pipe error");  // 如果创建管道失败,打印错误并返回-1
            return (-1);
        }

        // 创建子进程
        if ((pid = fork()) < 0)
        {
            err_ret("fork error");  // 如果创建子进程失败,打印错误并返回-1
            return (-1);
        }
        else if (pid == 0)
        {                  /* 子进程 */
            close(fd[0]);  // 关闭子进程的读端

            // 如果fd[1]不是标准输入,则将fd[1]复制到标准输入
            // 以下代码段用于将文件描述符fd[1]的内容复制到标准输入(STDIN_FILENO),
            // 如果fd[1]不是标准输入,并且复制操作失败,则调用err_sys函数报告错误.
            // 这通常用于重定向标准输入到一个文件或其他输入源.

            if (fd[1] != STDIN_FILENO &&                    // 检查fd[1]是否已经是标准输入
                dup2(fd[1], STDIN_FILENO) != STDIN_FILENO)  // 尝试复制fd[1]到标准输入
                err_sys("dup2 error to stdin");             // 如果复制失败,报告错误

            // 如果fd[1]不是标准输出,则将fd[1]复制到标准输出
            // 以下代码段检查文件描述符fd[1]是否不等于标准输出文件描述符STDOUT_FILENO,
            // 如果是,则尝试将fd[1]复制到STDOUT_FILENO.如果复制失败,则调用err_sys函数报告错误.
            // 这通常用于重定向标准输出到一个文件或管道.
            if (fd[1] != STDOUT_FILENO &&
                dup2(fd[1], STDOUT_FILENO) != STDOUT_FILENO)
                err_sys("dup2 error to stdout");

            // 执行opend程序
            if (execl("./opend", "opend", (char *)0) < 0)
                err_sys("execl error");  // 如果执行失败,打印错误
        }
        close(fd[1]); /* 父进程关闭写端 */
    }
    // 使用sprintf函数将oflag的值转换为ASCII字符串并存储在buf中
    sprintf(buf, " %d", oflag); /* oflag to ascii */

    // 初始化iov数组的第一个元素,包含字符串"CL_OPEN ",并计算其长度
    iov[0].iov_base = CL_OPEN " "; /* string concatenation */
    iov[0].iov_len  = strlen(CL_OPEN) + 1;

    // 初始化iov数组的第二个元素,包含文件名name,并计算其长度
    iov[1].iov_base = name;
    iov[1].iov_len  = strlen(name);

    // 初始化iov数组的第三个元素,包含之前转换的ASCII字符串buf,并计算其长度(包括结尾的空字符)
    iov[2].iov_base = buf;
    iov[2].iov_len  = strlen(buf) + 1; /* +1 for null at end of buf */

    // 计算iov数组中所有元素的总长度
    len = iov[0].iov_len + iov[1].iov_len + iov[2].iov_len;

    // 使用writev函数将iov数组中的数据写入文件描述符fd[0],并检查返回值是否等于总长度
    if (writev(fd[0], &iov[0], 3) != len)
    {
        // 如果写入的字节数不等于总长度,则输出错误信息并返回-1
        err_ret("writev error");
        return (-1);
    }

    /* read descriptor, returned errors handled by write() */
    // 调用recv_fd函数从文件描述符fd[0]读取描述符,并处理可能出现的错误
    return (recv_fd(fd[0], write));
}
