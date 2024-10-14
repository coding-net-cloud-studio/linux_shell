#include "apue.h"     // 引入自定义的apue库,可能包含一些实用函数
#include <termios.h>  // 引入终端控制接口,用于设置和获取终端属性
#include <errno.h>    // 引入错误处理头文件,定义了各种错误代码

// 定义一个静态的termios结构体变量save_termios,用于保存终端的原始设置
static struct termios save_termios;

// 定义一个静态整型变量ttysavefd,初始值为-1,用于保存终端文件描述符
static int ttysavefd = -1;

// 定义一个枚举类型,包含三个状态:RESET(重置),RAW(原始模式),CBREAK(连续中断模式)
// ttystate变量用于跟踪当前的终端状态
static enum { RESET,
              RAW,
              CBREAK } ttystate = RESET;

int tty_cbreak(int fd) /* put terminal into a cbreak mode */
{

    // 定义整型变量err,用于存储错误码
    int err;

    // 定义结构体变量buf,类型为termios,通常用于终端I/O设置
    struct termios buf;

    // 检查终端状态是否已经设置,如果是则返回错误
    if (ttystate != RESET)
    {
        errno = EINVAL;
        return (-1);
    }
    // 获取当前终端属性

    // 获取文件描述符fd对应的终端属性,并将结果存储在buf结构体中.
    // 如果调用失败(返回值小于0),则返回-1.
    if (tcgetattr(fd, &buf) < 0)
        return (-1);

    // 将当前的终端属性结构体buf复制到save_termios变量中,以便后续恢复终端设置.
    save_termios = buf; /* structure copy */

    /*
     * 关闭回显,关闭规范模式.
     */
    buf.c_lflag &= ~(ECHO | ICANON);

    /*
     * 模式B:一次一个字节,没有定时器.
     */
    // 设置终端属性结构体中的VMIN成员为1,表示至少需要读取1个字符才返回
    buf.c_cc[VMIN] = 1;

    // 设置终端属性结构体中的VTIME成员为0,表示没有超时时间,即非阻塞模式
    buf.c_cc[VTIME] = 0;

    // 设置新的终端属性,TCSAFLUSH表示在设置属性时清空输入输出队列
    // 如果设置失败,返回-1
    if (tcsetattr(fd, TCSAFLUSH, &buf) < 0)
        return (-1);

    /*
     * 验证更改是否生效.tcsetattr可能会部分成功返回0.
     */
    // 获取当前终端属性
    if (tcgetattr(fd, &buf) < 0)
    {
        // 如果获取失败,保存错误码
        err = errno;
        // 恢复原始的终端属性
        tcsetattr(fd, TCSAFLUSH, &save_termios);
        // 恢复原来的错误码
        errno = err;
        // 返回错误
        return (-1);
    }
    // 检查是否所有设置都已正确应用,如果不是则恢复原始设置
    if ((buf.c_lflag & (ECHO | ICANON)) || buf.c_cc[VMIN] != 1 ||
        buf.c_cc[VTIME] != 0)
    {
        // 如果设置不正确,恢复原始的终端属性
        tcsetattr(fd, TCSAFLUSH, &save_termios);
        // 设置错误码为无效参数
        errno = EINVAL;
        // 返回错误
        return (-1);
    }

    // 更新终端状态并保存文件描述符
    // 设置终端为CBREAK模式,这种模式下,输入的每个字符都会立即被传输到程序,不会等待回车键
    // ttystate 变量用于存储终端状态
    ttystate = CBREAK;

    // ttysavefd 变量用于保存文件描述符,以便之后恢复终端状态
    ttysavefd = fd;

    // 返回0表示操作成功
    return (0);
}

int tty_raw(int fd) /* 将终端设置为原始模式 */
{
    int            err;  // 错误码
    struct termios buf;  // 终端属性结构体

    if (ttystate != RESET)  // 如果终端状态不是重置状态
    {
        errno = EINVAL;  // 设置错误码为无效参数
        return (-1);     // 返回错误
    }
    if (tcgetattr(fd, &buf) < 0)  // 获取终端属性失败
        return (-1);              // 返回错误
    save_termios = buf;           // 保存原始终端属性

    // 关闭回显,关闭规范模式,关闭扩展输入处理,关闭信号字符
    buf.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);

    // BREAK不产生SIGINT,关闭CR到NL转换,关闭输入奇偶校验,不剥离输入的第8位,关闭输出流控制
    // 清除buf.c_iflag中的BRKINT,ICRNL,INPCK,ISTRIP和IXON标志位
    // 这样做可以禁用中断信号,将输入中的CR转换为NL,启用输入奇偶校验检查,
    // 剥离输入字节中的第8位以及启用软件流控制
    buf.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);

    // 清除大小位,关闭奇偶校验
    buf.c_cflag &= ~(CSIZE | PARENB);

    // 设置每个字符8位
    buf.c_cflag |= CS8;

    // 关闭输出处理
    buf.c_oflag &= ~(OPOST);

    // 每次读取1字节,不使用定时器
    // 设置终端属性,确保输入缓冲区至少有一个字符时才读取
    buf.c_cc[VMIN]  = 1;  // VMIN: 最小输入字符数
    buf.c_cc[VTIME] = 0;  // VTIME: 等待时间(十分之一秒),0表示立即返回

    // 尝试设置终端属性,TCSAFLUSH表示在设置属性前清空输入输出缓冲区
    if (tcsetattr(fd, TCSAFLUSH, &buf) < 0)
    {
        // 如果设置失败,返回错误码
        return (-1);
    }

    // 验证更改是否生效
    if (tcgetattr(fd, &buf) < 0)
    {
        err = errno;                              // 保存当前错误码
        tcsetattr(fd, TCSAFLUSH, &save_termios);  // 恢复原始终端属性
        errno = err;                              // 恢复错误码
        return (-1);                              // 返回错误
    }
    // 检查所有设置是否正确应用
    // 检查终端设置是否符合要求
    if ((buf.c_lflag & (ECHO | ICANON | IEXTEN | ISIG)) ||           // 检查本地标志是否包含不需要的项目
        (buf.c_iflag & (BRKINT | ICRNL | INPCK | ISTRIP | IXON)) ||  // 检查输入标志是否包含不需要的项目
        (buf.c_cflag & (CSIZE | PARENB | CS8)) != CS8 ||             // 检查控制标志中的字符大小是否为8位,无奇偶校验
        (buf.c_oflag & OPOST) ||                                     // 检查输出标志是否启用了转换
        buf.c_cc[VMIN] != 1 ||                                       // 检查最小输入字符数是否为1
        buf.c_cc[VTIME] != 0)                                        // 检查等待时间是否为0
    {
        // 如果只有部分更改生效,恢复原始设置
        tcsetattr(fd, TCSAFLUSH, &save_termios);  // 恢复终端的原始属性
        errno = EINVAL;                           // 设置错误码为无效参数
        return (-1);                              // 返回错误
    }

    ttystate  = RAW;  // 设置终端状态为原始模式
    ttysavefd = fd;   // 保存文件描述符
    return (0);       // 返回成功
}

// tty_reset函数用于恢复终端的原始模式
// 参数:
//   fd: 终端的文件描述符
// 返回值:
//   成功返回0,失败返回-1
int tty_reset(int fd) /* restore terminal's mode */
{
    // 如果终端状态已经是重置状态,则直接返回成功
    if (ttystate == RESET)
        return (0);

    // 使用tcsetattr函数尝试将终端属性设置为之前保存的状态
    // TCSAFLUSH标志表示在设置属性前刷新输入输出队列
    if (tcsetattr(fd, TCSAFLUSH, &save_termios) < 0)
        // 如果设置失败,返回-1
        return (-1);

    // 如果设置成功,更新终端状态为重置状态
    ttystate = RESET;

    // 返回成功
    return (0);
}

// tty_atexit 函数用于在程序退出时重置终端设置
// 该函数可以通过 atexit(tty_atexit) 注册,以便在程序退出时自动调用
void tty_atexit(void)
{
    // 检查 ttysavefd 是否已经保存了一个有效的文件描述符
    if (ttysavefd >= 0)
        // 如果 ttysavefd 有效,则调用 tty_reset 函数重置终端
        tty_reset(ttysavefd);
}

// tty_termios 函数用于返回一个指向保存的 tty 状态结构体的指针,以便调用者可以查看原始的 tty 状态.
// 该函数不接受任何参数,并返回一个指向 struct termios 类型的指针.
struct termios *
tty_termios(void) /* let caller see original tty state */
{
    // 返回指向保存的 tty 状态结构体的指针
    return (&save_termios);
}
