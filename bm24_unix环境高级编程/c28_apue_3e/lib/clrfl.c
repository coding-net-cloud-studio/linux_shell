#include "apue.h"   // 引入apue库,提供系统调用和实用程序函数的封装
#include <fcntl.h>  // 引入文件控制相关的头文件,如open, close, fcntl等函数

// @brief 关闭文件状态标志
// @param fd 文件描述符
// @param flags 要关闭的文件状态标志
// @return 无返回值,若出错则调用err_sys输出错误信息
void clr_fl(int fd, int flags)
/* flags are the file status flags to turn off */
{
    int val;

    // 获取当前文件状态标志
    // 从文件描述符fd获取当前的文件状态标志
    if ((val = fcntl(fd, F_GETFL, 0)) < 0)
        // 如果获取失败,调用err_sys函数报告系统错误
        err_sys("fcntl F_GETFL error");

    // 关闭指定的文件状态标志
    val &= ~flags; /* turn flags off */

    // 设置文件状态标志
    // 使用fcntl函数设置文件描述符fd的状态标志
    // F_SETFL: 设置文件状态标志
    // val: 新的状态标志
    if (fcntl(fd, F_SETFL, val) < 0)
        // 如果fcntl调用失败,输出错误信息
        err_sys("fcntl F_SETFL error");
}
