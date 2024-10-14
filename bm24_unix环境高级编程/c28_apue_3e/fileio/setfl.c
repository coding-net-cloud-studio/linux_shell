#include "apue.h"
#include <fcntl.h>

// @brief 设置文件状态标志
// @param fd 文件描述符
// @param flags 要打开的文件状态标志
// @return 无返回值,若出错则调用err_sys输出错误信息
void set_fl(int fd, int flags) /* flags are file status flags to turn on */
{
    int val;

    // 获取当前文件状态标志
    if ((val = fcntl(fd, F_GETFL, 0)) < 0)
        err_sys("fcntl F_GETFL error");

    // 将新的标志位与当前标志位进行或操作,以打开新的标志位
    val |= flags; /* turn on flags */

    // 设置文件状态标志
    if (fcntl(fd, F_SETFL, val) < 0)
        err_sys("fcntl F_SETFL error");
}
