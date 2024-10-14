#include "apue.h"   // 引入自定义的应用程序环境库
#include <errno.h>  // 引入错误处理头文件
#include <fcntl.h>  // 引入文件控制头文件

#if defined(SOLARIS)
#include <stropts.h>  // 如果定义了SOLARIS,则引入流操作头文件
#endif

// 以下代码段没有具体的函数方法,因此无需添加标准文档注释.
// 代码段的注释应该描述这段代码的作用和它在程序中的上下文.
// 例如:
// 这段代码包含了几个头文件,这些头文件提供了程序所需的系统级功能.
// "apue.h" 是一个自定义的头文件,可能包含了程序特定的函数和宏定义.
// <errno.h> 提供了错误处理的机制.
// <fcntl.h> 提供了文件打开,关闭,读写等操作的函数原型.
// 如果代码运行在SOLARIS系统上,<stropts.h> 提供了流操作的相关函数和数据结构.

/**
 * @brief 打开一个伪终端的主设备
 *
 * @param pts_name 用于存储从设备名称的缓冲区
 * @param pts_namesz pts_name 缓冲区的大小
 * @return int 成功时返回主设备的文件描述符,失败时返回 -1
 */
int ptym_open(char *pts_name, int pts_namesz)
{
    char *ptr;       // 用于存储从设备名称的指针
    int   fdm, err;  // fdm 是主设备的文件描述符,err 用于存储错误码

    // 打开伪终端的主设备
    if ((fdm = posix_openpt(O_RDWR)) < 0)
        return (-1);

    // 授权从设备访问
    if (grantpt(fdm) < 0)
        goto errout;

    // 清除从设备的锁定标志
    if (unlockpt(fdm) < 0)
        goto errout;

    // 获取从设备的名称
    if ((ptr = ptsname(fdm)) == NULL)
        goto errout;

    // 将从设备的名称复制到 pts_name 缓冲区,并确保字符串以空字符结尾
    strncpy(pts_name, ptr, pts_namesz);
    pts_name[pts_namesz - 1] = '\0';

    // 返回主设备的文件描述符
    return (fdm);

// errout: 错误处理标签
// 如果发生错误,此标签下的代码将被执行
// 1. 恢复之前保存的errno值
// 2. 关闭主设备文件描述符
// 3. 将errno设置为之前保存的错误码
// 4. 返回-1表示失败
errout:
    err = errno;  // 保存当前errno值
    close(fdm);   // 关闭文件描述符
    errno = err;  // 恢复errno值
    return (-1);  // 返回错误码
}

int ptys_open(char *pts_name)
{
    int fds;  // 文件描述符
#if defined(SOLARIS)
    int err, setup;  // 错误码和设置状态
#endif

    // 尝试以读写模式打开伪终端主设备
    if ((fds = open(pts_name, O_RDWR)) < 0)
        return (-1);  // 打开失败则返回-1

#if defined(SOLARIS)
    /*
     * 检查流是否已经通过autopush设施设置好.
     */
    // 使用ioctl检查是否已经设置ldterm
    if ((setup = ioctl(fds, I_FIND, "ldterm")) < 0)
        goto errout;  // 设置失败则跳转到错误处理

    // 如果ldterm未设置
    if (setup == 0)
    {
        // 推送ptem模块
        if (ioctl(fds, I_PUSH, "ptem") < 0)
            goto errout;
        // 推送ldterm模块
        if (ioctl(fds, I_PUSH, "ldterm") < 0)
            goto errout;
        // 推送ttcompat模块
        if (ioctl(fds, I_PUSH, "ttcompat") < 0)
        {
        errout:
            err = errno;  // 保存错误码
            close(fds);   // 关闭文件描述符
            errno = err;  // 恢复错误码
            return (-1);  // 返回-1表示失败
        }
    }
#endif
    return (fds);  // 成功打开并设置后返回文件描述符
}
