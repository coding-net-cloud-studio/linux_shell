#include <termios.h>

// isatty函数用于检查给定的文件描述符是否指向一个终端设备.
// 参数:
//   fd - 文件描述符
// 返回值:
//   如果fd指向一个终端设备,则返回非零值;否则返回0.
int isatty(int fd)
{
    struct termios ts;  // 定义一个termios结构体用于存储终端属性

    // 使用tcgetattr函数尝试获取文件描述符fd的终端属性
    // 如果成功获取(即没有错误),则返回非零值,表示fd指向一个终端设备
    return (tcgetattr(fd, &ts) != -1); /* true if no error (is a tty) */
}
