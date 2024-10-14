#include "apue.h"   // 引入自定义的应用程序环境头文件
#include <errno.h>  // 引入错误处理头文件
#include <fcntl.h>  // 引入文件控制头文件
#if defined(SOLARIS)
#include <stropts.h>  // 如果定义了SOLARIS,则引入流控制头文件
#endif

/*
 * ptym_open - 打开一个伪终端主设备,并返回从设备的名称
 *
 * @pts_name: 用于存储从设备名称的缓冲区
 * @pts_namesz: pts_name缓冲区的大小
 *
 * 返回值: 成功时返回主设备的文件描述符,失败时返回-1
 */
int ptym_open(char *pts_name, int pts_namesz)
{
    char *ptr;       // 用于存储从设备名称的指针
    int   fdm, err;  // fdm为主设备的文件描述符,err用于存储错误码

    // 打开伪终端主设备
    // 打开一个伪终端主设备,以便进行读写操作
    // posix_openpt() 函数尝试打开一个伪终端主设备
    // 参数 O_RDWR 表示以读写模式打开设备
    // 如果成功,函数返回文件描述符;如果失败,返回 -1
    if ((fdm = posix_openpt(O_RDWR)) < 0)
        // 如果打开失败,返回 -1 表示错误
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

    // 将从设备名称复制到pts_name,并确保字符串以空字符结尾
    // 从ptr指向的内存位置复制最多pts_namesz-1个字符到pts_name数组中
    // 确保字符串以空字符结尾,防止缓冲区溢出
    strncpy(pts_name, ptr, pts_namesz);
    // 在复制的字符串末尾添加空字符,确保其正确终止
    pts_name[pts_namesz - 1] = '\0';

    // 返回主设备的文件描述符
    return (fdm);

errout:
    // 发生错误时,保存错误码,关闭文件描述符,并恢复errno
    // 保存当前的errno值,以便在关闭文件描述符后恢复
    err = errno;
    // 关闭文件描述符fdm
    close(fdm);
    // 恢复之前保存的errno值
    errno = err;
    // 返回-1表示操作失败
    return (-1);
}

/**
 * @brief 打开一个伪终端主设备
 *
 * @param pts_name 伪终端名称
 * @return int 成功返回文件描述符,失败返回-1
 */
int ptys_open(char *pts_name)
{
    int fds;  // 文件描述符
#if defined(SOLARIS)
    int err, setup;  // 错误码和设置状态
#endif

    // 尝试以读写模式打开伪终端主设备
    if ((fds = open(pts_name, O_RDWR)) < 0)
        return (-1);  // 打开失败返回-1

#if defined(SOLARIS)
    /*
     * 检查流是否已经通过autopush设施设置好.
     */
    // 使用ioctl检查ldterm是否已设置
    if ((setup = ioctl(fds, I_FIND, "ldterm")) < 0)
        goto errout;  // 设置失败则跳转到错误处理

    // 如果ldterm未设置,则进行设置
    // 如果setup等于0,则执行以下操作
    if (setup == 0)
    {
        // 推送ptem模块到设备
        // 如果ioctl调用失败(返回值小于0),则跳转到errout标签处理错误
        if (ioctl(fds, I_PUSH, "ptem") < 0)
            goto errout;

        // 推送ldterm模块到设备
        // 如果ioctl调用失败(返回值小于0),则跳转到errout标签处理错误
        if (ioctl(fds, I_PUSH, "ldterm") < 0)
            goto errout;

        // 推送ttcompat模块到设备
        // 如果ioctl调用失败(返回值小于0),则跳转到errout标签处理错误
        if (ioctl(fds, I_PUSH, "ttcompat") < 0)
        {
        errout:
            // 保存当前错误码
            err = errno;
            // 关闭文件描述符
            close(fds);
            // 恢复错误码
            errno = err;
            // 返回错误
            return (-1);
        }
    }

#endif
    return (fds);  // 成功打开并设置后返回文件描述符
}
