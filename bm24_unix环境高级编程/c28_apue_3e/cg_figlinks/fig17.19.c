#include "open.h"     // 包含自定义的头文件open.h,可能包含文件打开相关的函数声明或宏定义
#include <sys/uio.h>  // 包含sys/uio.h头文件,定义了用于散布/聚集I/O的结构体iovec

/*
 * Open the file by sending the "name" and "oflag" to the
 * connection server and reading a file descriptor back.
 */
/*
 * 打开文件,通过向连接服务器发送"name"和"oflag",然后读回一个文件描述符.
 */

// csopen函数尝试打开一个文件,通过与远程的"opend"服务进程通信来实现.
// 参数name是要打开的文件的路径,oflag是打开文件时的标志.
int csopen(char *name, int oflag)
{
    pid_t        pid;               // 用于存储fork操作后子进程的PID
    int          len;               // 用于存储将要发送的消息的总长度
    char         buf[10];           // 缓冲区,用于存储将oflag转换为字符串后的结果
    struct iovec iov[3];            // 用于存储要发送的消息的iovec结构数组
    static int   fd[2] = {-1, -1};  // 文件描述符数组,用于存储管道的读写端

    // 如果管道尚未初始化,则初始化管道并启动opend服务进程
    if (fd[0] < 0)
    {
        if (fd_pipe(fd) < 0)  // 创建管道
        {
            err_ret("fd_pipe error");
            return (-1);
        }
        if ((pid = fork()) < 0)  // 创建子进程
        {
            err_ret("fork error");
            return (-1);
        }
        else if (pid == 0)  // 子进程执行的代码
        {
            close(fd[0]);  // 关闭子进程的读端,因为子进程不需要从管道读取数据
            // 将管道的写端重定向到标准输入输出,这样opend服务进程就可以通过标准输入输出与父进程通信
            if (fd[1] != STDIN_FILENO &&                      // 如果管道写端不是标准输入
                dup2(fd[1], STDIN_FILENO) != STDIN_FILENO)    // 将管道写端复制到标准输入
                err_sys("dup2 error to stdin");               // 如果出错,打印错误信息
            if (fd[1] != STDOUT_FILENO &&                     // 如果管道写端不是标准输出
                dup2(fd[1], STDOUT_FILENO) != STDOUT_FILENO)  // 将管道写端复制到标准输出
                err_sys("dup2 error to stdout");              // 如果出错,打印错误信息
            // 执行opend服务进程,替换当前进程映像为opend程序
            if (execl("./opend", "opend", (char *)0) < 0)
                err_sys("execl error");  // 如果execl调用失败,打印错误信息
        }
        close(fd[1]);  // 父进程关闭管道的写端
    }

    // 将oflag转换为字符串形式,准备发送
    // 使用sprintf函数将整数oflag格式化为字符串,并存储在buf中,后面跟一个空格
    sprintf(buf, " %d", oflag);

    // 设置第一个iovec结构
    // iov[0].iov_base指向字符串"CL_OPEN "的首地址,包含命令字符串"CL_OPEN"和空格
    // iov[0].iov_len是"CL_OPEN "的长度加1,包括末尾的空字符
    iov[0].iov_base = CL_OPEN " ";
    iov[0].iov_len  = strlen(CL_OPEN) + 1;

    // 设置第二个iovec结构
    // iov[1].iov_base指向文件名name的首地址
    // iov[1].iov_len是文件名name的长度
    iov[1].iov_base = name;
    iov[1].iov_len  = strlen(name);

    // 设置第三个iovec结构
    // iov[2].iov_base指向buf的首地址,buf中存储了oflag的字符串形式
    // iov[2].iov_len是buf的长度加1,包括末尾的空字符
    iov[2].iov_base = buf;
    iov[2].iov_len  = strlen(buf) + 1;  // +1是为了buf末尾的空字符

    len = iov[0].iov_len + iov[1].iov_len + iov[2].iov_len;  // 计算总长度
    // 发送消息到opend服务进程
    // 以下代码段用于向文件描述符fd[0]写入数据,使用writev函数同时写入iov数组中的3个数据块.
    // 如果写入的字节数不等于预期的len,则输出错误信息并返回-1.

    if (writev(fd[0], &iov[0], 3) != len)  // 使用writev函数尝试写入数据
    {
        err_ret("writev error");  // 如果写入的字节数不等于len,输出错误信息
        return (-1);              // 返回错误码-1
    }

    // 读回文件描述符,错误处理由write()函数完成
    return (recv_fd(fd[0], write));
}
