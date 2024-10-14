#include "apue.h"
#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>

#define QLEN 10

/*
 * 创建一个服务器端点连接.
 * 如果一切正常,返回文件描述符fd,否则返回<0表示错误.
 */
int serv_listen(const char *name)
{
    int                fd, len, err, rval;  // 定义文件描述符,长度,错误号和返回值变量
    struct sockaddr_un un;                  // 定义UNIX域套接字地址结构体

    // 检查名称长度是否超过结构体中路径字段的最大长度
    if (strlen(name) >= sizeof(un.sun_path))
    {
        errno = ENAMETOOLONG;  // 设置错误号为名称过长
        return (-1);
    }

    // 创建一个UNIX域流套接字
    if ((fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
        return (-2);  // 创建失败返回-2

    unlink(name);  // 如果套接字文件已存在,则先删除

    // 填充套接字地址结构体
    memset(&un, 0, sizeof(un));                                   // 清零结构体
    un.sun_family = AF_UNIX;                                      // 设置地址族为UNIX域
    strcpy(un.sun_path, name);                                    // 复制服务名称到路径字段
    len = offsetof(struct sockaddr_un, sun_path) + strlen(name);  // 计算地址结构体的长度

    // 将套接字绑定到指定的地址
    if (bind(fd, (struct sockaddr *)&un, len) < 0)
    {
        rval = -3;    // 绑定失败设置返回值为-3
        goto errout;  // 跳转到错误处理
    }

    // 开始监听套接字,设置最大连接队列长度
    if (listen(fd, QLEN) < 0)
    {                 // 监听失败
        rval = -4;    // 设置返回值为-4
        goto errout;  // 跳转到错误处理
    }
    return (fd);  // 监听成功,返回文件描述符

errout:
    err = errno;    // 保存当前错误号
    close(fd);      // 关闭文件描述符
    errno = err;    // 恢复错误号
    return (rval);  // 返回错误码
}
