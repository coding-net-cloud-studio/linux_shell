#include "apue.h"     // 引入apue库,提供系统调用和实用函数的封装
#include <termios.h>  // 引入termios库,用于控制终端I/O属性

pid_t pty_fork(int *ptrfdm, char *slave_name, int slave_namesz, const struct termios *slave_termios, const struct winsize *slave_winsize)
{
    int   fdm, fds;      // 定义文件描述符fdm和fds,分别用于主从伪终端
    pid_t pid;           // 定义进程ID
    char  pts_name[20];  // 定义存储伪终端名称的字符数组

    // 打开主伪终端,如果失败则输出错误信息并返回
    // 打开主伪终端
    // 如果打开失败,记录错误信息并退出程序
    if ((fdm = ptym_open(pts_name, sizeof(pts_name))) < 0)
        err_sys("无法打开主伪终端:%s,错误:%d", pts_name, fdm);

    // 如果slave_name不为空,则将主伪终端的名称复制到slave_name中
    // 如果slave_name不为空,则复制pts_name的内容到slave_name中
    // 并确保复制后的字符串以空字符结尾
    if (slave_name != NULL)
    {
        // 使用strncpy函数复制指定长度的字符串
        // 这样可以防止缓冲区溢出
        strncpy(slave_name, pts_name, slave_namesz);
        // 确保字符串以空字符结尾
        slave_name[slave_namesz - 1] = '\0';
    }

    // 创建子进程,如果失败则返回-1
    if ((pid = fork()) < 0)
    {
        return (-1);
    }
    else if (pid == 0)
    {                      /* child */
        if (setsid() < 0)  // 子进程创建新的会话
            err_sys("setsid error");

        // 打开从伪终端,如果失败则输出错误信息并返回
        // 打开从设备(slave pty),如果打开失败,则通过err_sys函数报告错误.
        // ptys_open是一个假设存在的函数,用于打开一个伪终端从设备.
        // pts_name是伪终端从设备的名称.
        // 如果fds小于0,表示打开失败.
        if ((fds = ptys_open(pts_name)) < 0)
            err_sys("can't open slave pty");  // 使用err_sys函数报告打开失败的错误信息.

        close(fdm);  // 子进程中关闭主伪终端文件描述符

#if defined(BSD)
        // BSD系统下获取控制终端
        if (ioctl(fds, TIOCSCTTY, (char *)0) < 0)
            err_sys("TIOCSCTTY error");
#endif
        // 设置从伪终端的termios和窗口大小
        // 如果slave_termios不为空,则设置从设备的终端属性
        if (slave_termios != NULL)
        {
            // 使用tcsetattr函数立即应用slave_termios到从设备,如果失败则输出错误信息
            // 如果设置从设备(slave pty)的终端属性失败,则调用err_sys函数报告错误
            // tcsetattr函数用于设置打开的文件描述符的终端属性
            // TCSANOW参数表示立即应用新的终端属性
            if (tcsetattr(fds, TCSANOW, slave_termios) < 0)
                err_sys("tcsetattr error on slave pty");  // 错误处理
        }
        // 如果slave_winsize不为空,则设置从设备的窗口大小
        if (slave_winsize != NULL)
        {
            // 使用ioctl函数设置从设备的窗口大小,如果失败则输出错误信息
            // 如果ioctl系统调用失败,报告在从设备pty上的TIOCSWINSZ错误
            // TIOCSWINSZ: 设置窗口大小
            // slave_winsize: 指向窗口大小结构的指针
            if (ioctl(fds, TIOCSWINSZ, slave_winsize) < 0)
                err_sys("TIOCSWINSZ error on slave pty");
        }

        // 将从伪终端的文件描述符复制到标准输入输出错误流
        // 将伪终端的文件描述符复制到标准输入,输出和错误流
        if (dup2(fds, STDIN_FILENO) != STDIN_FILENO)    // 复制到标准输入
            err_sys("dup2 error to stdin");             // 如果出错,打印错误信息
        if (dup2(fds, STDOUT_FILENO) != STDOUT_FILENO)  // 复制到标准输出
            err_sys("dup2 error to stdout");            // 如果出错,打印错误信息
        if (dup2(fds, STDERR_FILENO) != STDERR_FILENO)  // 复制到标准错误
            err_sys("dup2 error to stderr");            // 如果出错,打印错误信息

        // 如果fds不是标准输入,输出或错误流的文件描述符,则关闭它
        if (fds != STDIN_FILENO && fds != STDOUT_FILENO &&
            fds != STDERR_FILENO)
            close(fds);  // 关闭多余的从伪终端文件描述符

        return (0);  // 子进程返回0
    }
    else
    {                   /* parent */
        *ptrfdm = fdm;  // 父进程中将主伪终端文件描述符返回
        return (pid);   // 父进程返回子进程ID
    }
}
