#include "apue.h"        // 引入APUI库的头文件,提供系统调用和标准C库函数的封装
#include <sys/socket.h>  // 包含套接字编程所需的头文件,定义了socket函数和相关结构体
#include <sys/un.h>      // 包含UNIX域套接字定义的头文件,用于本地进程间通信
#include <time.h>        // 包含时间和日期函数的头文件,提供时间获取和格式化功能
#include <errno.h>       // 包含错误处理函数的头文件,定义了错误码和错误处理机制

#define STALE 30 /* 客户端的名称不能超过这个时间(秒) */

/*
 * Wait for a client connection to arrive, and accept it.
 * We also obtain the client's user ID from the pathname
 * that it must bind before calling us.
 * Returns new fd if all OK, <0 on error
 */

/*
 * 等待客户端连接到达并接受它.
 * 我们还从它在调用我们之前必须绑定的路径名中获取客户端的用户ID.
 * 如果一切正常,返回新的文件描述符,如果出错,返回<0
 */

/**
 * @brief 接受一个来自监听套接字的连接请求,并返回一个新的套接字描述符.
 *
 * @param listenfd 监听套接字的描述符.
 * @param uidptr 指向用户ID的指针,用于存储连接请求的用户ID.
 * @return int 成功时返回新的套接字描述符;失败时返回-1,并设置errno.
 */
int serv_accept(int listenfd, uid_t *uidptr)
{
    int                clifd, err, rval;  // 定义客户端文件描述符,错误码和返回值
    socklen_t          len;               // 定义地址结构体长度
    time_t             staletime;         // 定义时间变量,用于检查文件状态是否过时
    struct sockaddr_un un;                // 定义UNIX域套接字地址结构体
    struct stat        statbuf;           // 定义文件状态结构体
    char              *name;              // 定义字符指针,用于存储客户端路径名

    /* 分配足够的空间来存储最长的名称加上终止的空字符 */

    // 分配内存以存储名称,大小为un.sun_path的长度加1
    if ((name = malloc(sizeof(un.sun_path + 1))) == NULL)
        return (-1);  // 如果内存分配失败,返回-1

    // 获取un结构体的大小
    len = sizeof(un);
    // 接受连接请求,如果失败则释放之前分配的内存并返回-2
    // accept函数用于接受一个连接请求,返回一个新的套接字描述符,用于与客户端通信.
    // listenfd: 监听套接字描述符.
    // (struct sockaddr *)&un: 指向客户端地址结构的指针.
    // &len: 客户端地址结构大小的指针.
    // 返回值: 成功时返回新的套接字描述符;失败时返回-1.
    if ((clifd = accept(listenfd, (struct sockaddr *)&un, &len)) < 0)

    {
        free(name);   // 释放之前分配的内存
        return (-2);  // 如果接受连接失败,返回-2
    }

    /* 从客户端的调用地址获取客户端的uid */
    len -= offsetof(struct sockaddr_un, sun_path);  // 获取路径名长度
    memcpy(name, un.sun_path, len);                 // 复制路径名到name
    name[len] = 0;                                  // 确保字符串以空字符结尾

    // 获取文件状态,如果失败则跳转到错误处理
    // 参数:
    //   name - 文件名
    //   statbuf - 用于存储文件状态信息的结构体指针
    // 返回值:
    //   成功返回0,失败返回-1
    if (stat(name, &statbuf) < 0)
    {
        rval = -3;    // 设置错误码
        goto errout;  // 跳转到错误处理标签
    }

#ifdef S_ISSOCK  // 检查S_ISSOCK是否定义
    if (S_ISSOCK(statbuf.st_mode) == 0)
    {
        rval = -4;    // 不是socket,返回-4
        goto errout;  // 跳转到错误处理
    }
#endif

    // 检查文件权限是否正确
    // 检查文件权限是否符合要求
    // 如果组权限或其它权限被设置,或者用户权限不全为读写执行,则认为权限不正确
    if ((statbuf.st_mode & (S_IRWXG | S_IRWXO)) ||  // 检查组权限或其它权限是否被设置
        (statbuf.st_mode & S_IRWXU) != S_IRWXU)     // 检查用户权限是否全为读写执行
    {
        rval = -5;    // 权限不正确,返回-5
        goto errout;  // 跳转到错误处理
    }

    staletime = time(NULL) - STALE;  // 计算文件状态过时的时间
    // 检查文件的访问,更改和修改时间是否过时
    // 检查文件的访问时间,创建时间和修改时间是否都晚于给定的时间
    if (statbuf.st_atime < staletime ||  // 如果访问时间早于staletime
        statbuf.st_ctime < staletime ||  // 如果创建时间早于staletime
        statbuf.st_mtime < staletime)    // 如果修改时间早于staletime
    {
        rval = -6;    // 文件状态过时,返回错误码-6
        goto errout;  // 跳转到错误处理部分
    }

    if (uidptr != NULL)
        *uidptr = statbuf.st_uid;  // 如果uidptr不为空,返回调用者的uid
    unlink(name);                  // 删除路径名,因为我们不再需要它
    free(name);                    // 释放之前分配的内存
    return (clifd);                // 返回客户端文件描述符

errout:
    err = errno;    // 保存当前错误码
    close(clifd);   // 关闭客户端文件描述符
    free(name);     // 释放之前分配的内存
    errno = err;    // 恢复原来的错误码
    return (rval);  // 返回错误码
}
