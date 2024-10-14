#include <signal.h>   // 包含信号处理函数库
#include <stdio.h>    // 包含标准输入输出函数库
#include <termios.h>  // 包含终端I/O控制函数库

#define MAX_PASS_LEN 8  // 定义用户输入密码的最大长度为8个字符

char *
getpass(const char *prompt)
{
    // buf数组用于存储用户输入的密码,长度限制为MAX_PASS_LEN,末尾添加空字节以表示字符串结束.
    static char buf[MAX_PASS_LEN + 1]; /* null byte at end */

    // ptr是一个指向字符的指针,可能用于操作buf数组.
    char *ptr;

    // sig和osig是信号集,用于在程序运行期间处理信号.
    sigset_t sig, osig;

    // ts和ots是termios结构体,用于控制终端的I/O特性.
    struct termios ts, ots;

    // fp是一个文件指针,可能用于读取或写入文件.
    FILE *fp;

    // c是一个整型变量,可能用于存储从终端读取的单个字符.
    int c;

    // 打开当前控制终端,准备读取密码
    if ((fp = fopen(ctermid(NULL), "r+")) == NULL)
        return (NULL);  // 如果打开失败,返回NULL
    setbuf(fp, NULL);   // 设置缓冲区为无缓冲

    // 设置信号集,阻塞SIGINT和SIGTSTP信号
    // 清空信号集
    sigemptyset(&sig);

    // 向信号集添加SIGINT信号,用于阻塞中断信号
    // SIGINT信号通常由用户按下Ctrl+C产生
    sigaddset(&sig, SIGINT);

    // 向信号集添加SIGTSTP信号,用于阻塞暂停信号
    // SIGTSTP信号通常由用户按下Ctrl+Z产生
    sigaddset(&sig, SIGTSTP);

    // 使用SIG_BLOCK标志调用sigprocmask函数,将信号集sig中的信号添加到进程的信号屏蔽字中
    // 同时保存当前的信号屏蔽字到osig中,以便之后可以恢复
    sigprocmask(SIG_BLOCK, &sig, &osig);

    // 获取当前终端属性并保存
    tcgetattr(fileno(fp), &ts);
    ots = ts;  // 复制终端属性结构体

    // 修改终端属性,关闭回显
    // 禁用终端的回显功能,包括:
    // ECHO: 启用输入字符的回显
    // ECHOE: 当接收到ERASE字符时,删除前一个字符
    // ECHOK: 当接收到KILL字符时,删除整行
    // ECHONL: 即使不是回车符,也回显新行
    ts.c_lflag &= ~(ECHO | ECHOE | ECHOK | ECHONL);

    tcsetattr(fileno(fp), TCSAFLUSH, &ts);  // 应用新的终端属性

    fputs(prompt, fp);  // 输出提示信息

    ptr = buf;  // 初始化密码缓冲区指针
    // 读取用户输入的密码,直到遇到EOF或换行符
    while ((c = getc(fp)) != EOF && c != '\n')
        if (ptr < &buf[MAX_PASS_LEN])  // 确保不超出缓冲区大小
            *ptr++ = c;                // 将字符存入缓冲区
    *ptr = 0;                          // 置空结束符
    putc('\n', fp);                    // 输出换行符,因为我们之前关闭了回显

    // 恢复终端属性
    // 使用tcsetattr函数将文件描述符fp所关联的终端属性恢复为ots结构体中保存的状态
    // TCSAFLUSH标志表示在设置属性后清空输入输出缓冲区
    tcsetattr(fileno(fp), TCSAFLUSH, &ots);

    // 恢复信号集
    // 使用sigprocmask函数将进程的信号屏蔽字设置为osig结构体中保存的信号集
    // SIG_SETMASK标志表示设置新的信号屏蔽字
    sigprocmask(SIG_SETMASK, &osig, NULL);

    fclose(fp);    // 关闭文件指针
    return (buf);  // 返回读取到的密码
}
