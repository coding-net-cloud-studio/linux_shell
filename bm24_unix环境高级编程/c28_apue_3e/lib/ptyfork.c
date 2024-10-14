#include "apue.h"     // 引入APUE库的头文件,该库提供了一套用于编写可移植Unix应用程序的函数.
#include <termios.h>  // 引入termios.h头文件,该文件包含了控制终端I/O的函数和结构体定义.

// @brief 通过伪终端创建子进程
// @param ptrfdm 指向主伪终端文件描述符的指针
// @param slave_name 指向从伪终端名称的指针
// @param slave_namesz 从伪终端名称的最大长度
// @param slave_termios 指向从伪终端的termios结构体的指针
// @param slave_winsize 指向从伪终端的winsize结构体的指针
// @return 成功返回子进程的pid,失败返回-1
pid_t pty_fork(int *ptrfdm, char *slave_name, int slave_namesz, const struct termios *slave_termios, const struct winsize *slave_winsize)
{
    int   fdm, fds;      // 主伪终端和从伪终端的文件描述符
    pid_t pid;           // 子进程的pid
    char  pts_name[20];  // 伪终端名称

    // 打开主伪终端
    // 打开主伪终端
    // 如果打开失败,使用err_sys函数报告错误
    if ((fdm = ptym_open(pts_name, sizeof(pts_name))) < 0)
        err_sys("无法打开主伪终端: %s, 错误码 %d", pts_name, fdm);

    // 如果提供了从伪终端名称,则复制名称
    // 如果slave_name不为空,则将pts_name的内容复制到slave_name中
    // 并确保slave_name以空字符结尾,以防止字符串溢出
    if (slave_name != NULL)
    {
        strncpy(slave_name, pts_name, slave_namesz);  // 复制字符串
        slave_name[slave_namesz - 1] = '\0';          // 确保字符串以空字符结尾
    }

    // 创建子进程
    if ((pid = fork()) < 0)
    {
        return (-1);
    }
    else if (pid == 0)
    {                      /* 子进程 */
        if (setsid() < 0)  // 创建新的会话
            err_sys("setsid error");

        // 打开从伪终端
        if ((fds = ptys_open(pts_name)) < 0)
            err_sys("can't open slave pty");
        close(fdm);  // 子进程中不再需要主伪终端

#if defined(BSD)
        // BSD系统获取控制终端的方式
        if (ioctl(fds, TIOCSCTTY, (char *)0) < 0)
            err_sys("TIOCSCTTY error");
#endif
        // 设置从伪终端的termios和窗口大小
        // 如果slave_termios不为空,则设置从设备的终端属性
        if (slave_termios != NULL)
        {
            if (tcsetattr(fds, TCSANOW, slave_termios) < 0)
                // 如果设置失败,打印错误信息
                err_sys("tcsetattr error on slave pty");
        }
        // 如果slave_winsize不为空,则设置从设备的窗口大小
        if (slave_winsize != NULL)
        {
            if (ioctl(fds, TIOCSWINSZ, slave_winsize) < 0)
                // 如果设置失败,打印错误信息
                err_sys("TIOCSWINSZ error on slave pty");
        }

        // 将从伪终端设置为子进程的标准输入/输出/错误
        // 以下代码段用于在子进程中重定向标准输入,输出和错误流

        // 将文件描述符fds复制到标准输入流,如果失败则调用err_sys函数报告错误
        if (dup2(fds, STDIN_FILENO) != STDIN_FILENO)
            err_sys("dup2 error to stdin");

        // 将文件描述符fds复制到标准输出流,如果失败则调用err_sys函数报告错误
        if (dup2(fds, STDOUT_FILENO) != STDOUT_FILENO)
            err_sys("dup2 error to stdout");

        // 将文件描述符fds复制到标准错误流,如果失败则调用err_sys函数报告错误
        if (dup2(fds, STDERR_FILENO) != STDERR_FILENO)
            err_sys("dup2 error to stderr");

        // 如果fds不是标准输入,输出或错误流的文件描述符,则关闭fds
        if (fds != STDIN_FILENO && fds != STDOUT_FILENO &&
            fds != STDERR_FILENO)
            close(fds);

        // 子进程返回0,表示成功完成
        return (0);
    }
    else
    {                   /* 父进程 */
        *ptrfdm = fdm;  // 返回主伪终端的文件描述符
        return (pid);   // 返回子进程的pid
    }
}
