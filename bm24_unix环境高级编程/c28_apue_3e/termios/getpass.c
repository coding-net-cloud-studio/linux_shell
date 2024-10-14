#include <signal.h>
#include <stdio.h>
#include <termios.h>

#define MAX_PASS_LEN 8 /* 用户输入的最大字符数 */

/*
 * getpass - 获取用户输入的密码,不在终端上显示
 * @prompt: 提示信息
 * 返回: 用户输入的密码字符串
 */
char *
getpass(const char *prompt)
{
    static char    buf[MAX_PASS_LEN + 1]; /* 存储密码的缓冲区,末尾有空字符 */
    char          *ptr;                   /* 指向当前填充位置的指针 */
    sigset_t       sig, osig;             /* 信号集,用于阻塞和恢复信号 */
    struct termios ts, ots;               /* 终端属性结构体,用于保存和恢复终端状态 */
    FILE          *fp;                    /* 文件指针,指向终端设备 */
    int            c;                     /* 从终端读取的字符 */

    if ((fp = fopen(ctermid(NULL), "r+")) == NULL) /* 打开终端设备 */
        return (NULL);
    setbuf(fp, NULL); /* 设置文件缓冲区为无缓冲 */

    sigemptyset(&sig);                   /* 清空信号集 */
    sigaddset(&sig, SIGINT);             /* 添加SIGINT信号到信号集 */
    sigaddset(&sig, SIGTSTP);            /* 添加SIGTSTP信号到信号集 */
    sigprocmask(SIG_BLOCK, &sig, &osig); /* 阻塞信号集中的信号,并保存原信号掩码 */

    tcgetattr(fileno(fp), &ts);                     /* 获取当前终端属性 */
    ots = ts;                                       /* 复制终端属性结构体 */
    ts.c_lflag &= ~(ECHO | ECHOE | ECHOK | ECHONL); /* 关闭终端的回显功能 */
    tcsetattr(fileno(fp), TCSAFLUSH, &ts);          /* 设置新的终端属性 */

    fputs(prompt, fp); /* 输出提示信息 */

    ptr = buf;                                 /* 初始化指针 */
    while ((c = getc(fp)) != EOF && c != '\n') /* 读取用户输入 */
        if (ptr < &buf[MAX_PASS_LEN])          /* 检查缓冲区是否已满 */
            *ptr++ = c;                        /* 存储字符到缓冲区 */
    *ptr = 0;                                  /* 空字符结尾 */
    putc('\n', fp);                            /* 输出换行符 */

    tcsetattr(fileno(fp), TCSAFLUSH, &ots); /* 恢复终端属性 */
    sigprocmask(SIG_SETMASK, &osig, NULL);  /* 恢复原信号掩码 */
    fclose(fp);                             /* 关闭终端设备文件 */
    return (buf);                           /* 返回密码字符串 */
}
