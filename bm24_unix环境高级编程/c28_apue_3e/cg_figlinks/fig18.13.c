#include <termios.h>  // 引入终端控制接口库,用于设置终端属性

// isatty函数用于检查文件描述符fd是否指向一个终端设备.
// 如果fd指向的是终端设备,则返回非零值;否则返回0.
//
// 参数:
//   fd - 要检查的文件描述符
//
// 返回值:
//   如果fd指向终端设备,返回非零值;否则返回0.
int isatty(int fd)
{
    struct termios ts;  // 定义一个termios结构体用于存储终端属性

    // 调用tcgetattr函数获取文件描述符fd的当前终端属性
    // 如果调用成功(没有错误),则返回非-1值,表示fd指向的是一个终端设备
    return (tcgetattr(fd, &ts) != -1); /* true if no error (is a tty) */
}
