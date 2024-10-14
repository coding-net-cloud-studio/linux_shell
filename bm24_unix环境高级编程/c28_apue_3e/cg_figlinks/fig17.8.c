#include "apue.h"        // 引入apue库,提供系统编程的API
#include <sys/socket.h>  // 引入套接字编程所需的头文件
#include <sys/un.h>      // 引入UNIX域套接字所需的头文件
#include <errno.h>       // 引入错误处理所需的头文件

#define QLEN 10  // 定义队列长度为10

/*
 * Create a server endpoint of a connection.
 * Returns fd if all OK, <0 on error.
 */

/*
 * 创建一个服务器端点连接.
 * 如果一切正常,返回文件描述符fd,出错则返回<0.
 */
int serv_listen(const char *name)
{
    int                fd, len, err, rval;  // 定义文件描述符fd,长度len,错误号err,返回值rval
    struct sockaddr_un un;                  // 定义UNIX域套接字地址结构体

    // 检查name的长度是否超过un.sun_path的最大长度
    if (strlen(name) >= sizeof(un.sun_path))
    {
        errno = ENAMETOOLONG;  // 设置错误号为名称过长
        return (-1);           // 返回错误
    }

    // 创建一个UNIX域流套接字
    if ((fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
        return (-2);  // 创建失败,返回错误

    unlink(name);  // 如果套接字文件已存在,则先删除

    // 填充套接字地址结构体
    memset(&un, 0, sizeof(un));                                   // 清零结构体
    un.sun_family = AF_UNIX;                                      // 设置地址族为UNIX域
    strcpy(un.sun_path, name);                                    // 复制name到sun_path
    len = offsetof(struct sockaddr_un, sun_path) + strlen(name);  // 计算地址结构体的长度

    // 将名称绑定到描述符
    if (bind(fd, (struct sockaddr *)&un, len) < 0)
    {
        rval = -3;    // 设置返回值为-3
        goto errout;  // 跳转到错误处理
    }

    // 告诉内核我们是一个服务器
    if (listen(fd, QLEN) < 0)
    {
        rval = -4;    // 设置返回值为-4
        goto errout;  // 跳转到错误处理
    }
    return (fd);  // 返回文件描述符

errout:
    err = errno;    // 保存当前错误号
    close(fd);      // 关闭文件描述符
    errno = err;    // 恢复错误号
    return (rval);  // 返回错误
}
