#include "apue.h"     // 引入apue库头文件
#include <termios.h>  // 引入终端控制头文件
#include <errno.h>    // 引入错误处理头文件

// 定义一个静态的termios结构体变量,用于保存原始终端设置
static struct termios save_termios;
// 定义一个静态整型变量,用于保存终端文件描述符,默认为-1
static int ttysavefd = -1;
// 定义一个枚举类型,表示终端的状态,包括重置状态,原始模式和CBREAK模式
static enum { RESET,
              RAW,
              CBREAK } ttystate = RESET;

int tty_cbreak(int fd) /* 将终端置于cbreak模式 */
{
    int            err;  // 错误码
    struct termios buf;  // 终端属性结构体

    if (ttystate != RESET)  // 如果终端状态不是重置状态
    {
        errno = EINVAL;  // 设置错误码为无效参数
        return (-1);     // 返回错误
    }
    if (tcgetattr(fd, &buf) < 0)  // 获取终端当前属性
        return (-1);              // 获取失败则返回错误
    save_termios = buf;           /* 结构体复制 */

    /*
     * 关闭回显,关闭规范模式.
     */
    buf.c_lflag &= ~(ECHO | ICANON);

    /*
     * 情况B:一次一个字节,无计时器.
     */
    buf.c_cc[VMIN]  = 1;                     // 最小输入字符数为1
    buf.c_cc[VTIME] = 0;                     // 读取超时时间为0
    if (tcsetattr(fd, TCSAFLUSH, &buf) < 0)  // 设置终端属性
        return (-1);                         // 设置失败则返回错误

    /*
     * 验证更改是否生效.tcsetattr可能会部分成功返回0.
     */
    if (tcgetattr(fd, &buf) < 0)  // 再次获取终端属性
    {
        err = errno;                              // 保存当前错误码
        tcsetattr(fd, TCSAFLUSH, &save_termios);  // 恢复原设置
        errno = err;                              // 恢复原错误码
        return (-1);                              // 返回错误
    }
    if ((buf.c_lflag & (ECHO | ICANON)) || buf.c_cc[VMIN] != 1 ||
        buf.c_cc[VTIME] != 0)  // 检查设置是否完全生效
    {
        /*
         * 只有部分更改被应用.恢复原始设置.
         */
        tcsetattr(fd, TCSAFLUSH, &save_termios);  // 恢复原设置
        errno = EINVAL;                           // 设置错误码为无效参数
        return (-1);                              // 返回错误
    }

    ttystate  = CBREAK;  // 更新终端状态为cbreak
    ttysavefd = fd;      // 保存文件描述符
    return (0);          // 成功返回
}

int tty_raw(int fd) /* 将终端设置为原始模式 */
{
    int            err;
    struct termios buf;

    // 检查终端状态是否已经是重置状态,如果不是,则返回错误
    if (ttystate != RESET)
    {
        errno = EINVAL;
        return (-1);
    }
    // 获取当前终端属性
    if (tcgetattr(fd, &buf) < 0)
        return (-1);
    save_termios = buf; /* 复制结构体 */

    // 关闭回显,规范模式,扩展输入处理和信号字符
    buf.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);

    // 关闭BREAK上的SIGINT,CR到NL转换,输入奇偶校验检查,输入时剥离第8位,输出流控制
    buf.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);

    // 清除大小位,关闭奇偶校验
    buf.c_cflag &= ~(CSIZE | PARENB);

    // 设置每个字符8位
    buf.c_cflag |= CS8;

    // 关闭输出处理
    buf.c_oflag &= ~(OPOST);

    // 设置每次读取1个字节,不使用定时器
    buf.c_cc[VMIN]  = 1;
    buf.c_cc[VTIME] = 0;
    // 应用新的终端属性,并刷新输出缓冲区
    if (tcsetattr(fd, TCSAFLUSH, &buf) < 0)
        return (-1);

    // 验证更改是否生效
    // tcgetattr函数用于获取终端设备的属性
    // 如果获取失败(返回值小于0),则执行以下操作:
    // 1. 保存当前的errno值到err变量
    // 2. 使用tcsetattr函数恢复终端设备的原始设置
    // 3. 将保存的errno值恢复到errno变量中
    // 4. 返回-1表示失败
    if (tcgetattr(fd, &buf) < 0)
    {
        err = errno;                              // 保存当前errno值
        tcsetattr(fd, TCSAFLUSH, &save_termios);  // 恢复原始终端设置
        errno = err;                              // 恢复保存的errno值
        return (-1);                              // 返回失败
    }

    // 检查所有设置是否正确应用,如果有任何设置未生效,则恢复原始设置
    // 检查终端设置是否符合要求
    if ((buf.c_lflag & (ECHO | ICANON | IEXTEN | ISIG)) ||           // 检查本地标志是否包含不需要的项目
        (buf.c_iflag & (BRKINT | ICRNL | INPCK | ISTRIP | IXON)) ||  // 检查输入标志是否包含不需要的项目
        (buf.c_cflag & (CSIZE | PARENB | CS8)) != CS8 ||             // 检查控制标志中的字符大小是否为8位,且没有奇偶校验
        (buf.c_oflag & OPOST) ||                                     // 检查输出标志是否包含不需要的项目
        buf.c_cc[VMIN] != 1 ||                                       // 检查最小输入字符数是否为1
        buf.c_cc[VTIME] != 0)                                        // 检查等待时间是否为0
    {
        tcsetattr(fd, TCSAFLUSH, &save_termios);  // 如果不符合要求,恢复终端的原始设置
        errno = EINVAL;                           // 设置错误码为无效参数
        return (-1);                              // 返回错误
    }

    ttystate  = RAW;  // 设置终端状态为原始模式
    ttysavefd = fd;   // 保存文件描述符
    return (0);
}

// tty_reset函数用于恢复终端的原始模式
// 参数:
//   fd: 终端的文件描述符
// 返回值:
//   成功返回0,失败返回-1
int tty_reset(int fd) /* restore terminal's mode */
{
    // 如果当前状态已经是RESET,则无需操作,直接返回成功
    if (ttystate == RESET)
        return (0);
    // 使用tcsetattr函数尝试将终端属性设置为之前保存的状态
    // TCSAFLUSH标志表示在设置属性前先刷新输入输出队列
    if (tcsetattr(fd, TCSAFLUSH, &save_termios) < 0)
        // 如果设置失败,返回-1
        return (-1);
    // 设置成功后,更新ttystate为RESET
    ttystate = RESET;
    // 返回成功
    return (0);
}

void tty_atexit(void) /* 可以通过 atexit(tty_atexit) 设置 */
{
    // 如果 ttysavefd 大于等于 0,表示之前保存了一个有效的文件描述符
    if (ttysavefd >= 0)
        // 调用 tty_reset 函数重置之前保存的终端设置
        tty_reset(ttysavefd);
}

// tty_termios 函数返回一个指向保存的 tty 状态的指针,允许调用者查看原始的 tty 状态.
// 该函数没有输入参数.
// 返回值:指向 struct termios 类型的指针,该结构体包含了 tty 的状态信息.
struct termios *
tty_termios(void) /* let caller see original tty state */
{
    return (&save_termios);  // 返回保存的 tty 状态的地址
}
