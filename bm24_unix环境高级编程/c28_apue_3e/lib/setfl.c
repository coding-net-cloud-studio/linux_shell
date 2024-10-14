#include "apue.h"   // 引入APUE库的头文件,该库提供了一套用于UNIX和类UNIX系统的应用程序编程接口
#include <fcntl.h>  // 引入文件控制操作的头文件,包括文件打开,关闭,锁定等操作

// 以下代码段没有具体的函数方法,因此无需添加标准文档注释

// @brief 设置文件状态标志
// @param fd 文件描述符
// @param flags 要打开的文件状态标志
void set_fl(int fd, int flags) /* flags are file status flags to turn on */
{
    int val;

    // @brief 获取当前文件状态标志
    if ((val = fcntl(fd, F_GETFL, 0)) < 0)
        err_sys("fcntl F_GETFL error");

    val |= flags; /* 打开标志 */

    // @brief 设置文件状态标志
    if (fcntl(fd, F_SETFL, val) < 0)
        err_sys("fcntl F_SETFL error");
}
