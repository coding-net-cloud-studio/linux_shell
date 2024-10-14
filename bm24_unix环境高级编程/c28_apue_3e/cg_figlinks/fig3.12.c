#include "apue.h"   // 引入APUE库,提供系统调用和实用函数的封装
#include <fcntl.h>  // 包含文件控制操作的头文件,如open, close等

// @brief 设置文件状态标志
// @param fd 文件描述符
// @param flags 要打开的文件状态标志
void set_fl(int fd, int flags) /* flags are file status flags to turn on */
{
    int val;

    // @brief 获取当前文件状态标志
    // 调用 fcntl 函数获取文件描述符 fd 的文件状态标志
    if ((val = fcntl(fd, F_GETFL, 0)) < 0)
        // 如果获取失败,调用 err_sys 函数输出错误信息并终止程序
        err_sys("fcntl F_GETFL error");

    // @brief 将新的标志位与当前标志位进行或操作,以打开新的标志位
    val |= flags; /* turn on flags */

    // @brief 设置文件的新的状态标志

    // 使用fcntl函数设置文件描述符fd的状态标志.
    // F_SETFL:设置文件状态标志.
    // val:新的文件状态标志.
    // 如果操作成功,fcntl返回新的文件状态标志;如果失败,返回-1.
    if (fcntl(fd, F_SETFL, val) < 0)
        // 如果fcntl调用失败,使用err_sys函数打印错误信息并退出程序.
        err_sys("fcntl F_SETFL error");
}
