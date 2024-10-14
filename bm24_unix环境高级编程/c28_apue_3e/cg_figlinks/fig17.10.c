#include "apue.h"        // 引入APUE库头文件
#include <sys/socket.h>  // 引入系统socket头文件
#include <sys/un.h>      // 引入UNIX域套接字头文件
#include <errno.h>       // 引入错误处理头文件

#define CLI_PATH "/var/tmp/"                      // 定义客户端套接字文件路径
#define CLI_PERM S_IRWXU /* rwx for user only */  // 定义客户端套接字文件的权限,仅用户可读写执行

/*
 * Create a client endpoint and connect to a server.
 * Returns fd if all OK, <0 on error.
 */

/*
 * 创建一个客户端端点并连接到服务器.
 * 如果一切正常,返回文件描述符fd,否则返回<0表示错误.
 */

int cli_conn(const char *name)
{
    int                fd, len, err, rval;  // 定义文件描述符,长度,错误码和返回值
    struct sockaddr_un un, sun;             // 定义UNIX域套接字地址结构
    int                do_unlink = 0;       // 标记是否需要删除文件

    // 检查名称长度是否超过限制
    // 检查name的长度是否大于等于un.sun_path的大小
    // 如果是,则设置errno为ENAMETOOLONG,表示文件名过长,并返回-1
    if (strlen(name) >= sizeof(un.sun_path))
    {
        errno = ENAMETOOLONG;  // 设置错误码为文件名过长
        return (-1);           // 返回错误码
    }

    // 创建一个UNIX域流套接字
    // 创建一个UNIX域套接字,用于本地进程间通信
    // AF_UNIX: 指定套接字家族为UNIX域套接字
    // SOCK_STREAM: 指定套接字类型为流式套接字,提供可靠的,面向连接的字节流
    // 0: 使用默认协议
    if ((fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
        // 如果socket创建失败,返回-1表示错误
        return (-1);

    // 填充套接字地址结构体,设置我们的地址
    // 初始化一个sockaddr_un结构体变量un,并将其清零
    memset(&un, 0, sizeof(un));

    // 设置地址族为UNIX域套接字
    un.sun_family = AF_UNIX;

    // 将CLI_PATH和当前进程ID格式化成一个新的路径字符串,并存储到un.sun_path中
    // %05ld表示以五位数字格式输出长整型数,不足五位时前面补零
    sprintf(un.sun_path, "%s%05ld", CLI_PATH, (long)getpid());

    // 打印出构造的文件路径
    printf("file is %s\n", un.sun_path);

    // 计算sun_path字段在sockaddr_un结构体中的偏移量加上sun_path字符串的长度,得到整个结构体的长度
    len = offsetof(struct sockaddr_un, sun_path) + strlen(un.sun_path);

    // 如果文件已存在,则删除它
    // 删除之前创建的套接字文件
    unlink(un.sun_path);

    // 尝试将套接字绑定到指定的地址
    if (bind(fd, (struct sockaddr *)&un, len) < 0)
    {
        rval = -2;    // 绑定失败,设置错误码
        goto errout;  // 跳转到错误处理部分
    }

    // 设置文件权限,确保客户端有权限连接
    if (chmod(un.sun_path, CLI_PERM) < 0)
    {
        rval      = -3;  // 设置权限失败,设置错误码
        do_unlink = 1;   // 标记需要删除套接字文件
        goto errout;     // 跳转到错误处理部分
    }

    // 填充套接字地址结构体,设置服务器的地址
    // 初始化sockaddr_un结构体
    memset(&sun, 0, sizeof(sun));  // 清零结构体
    sun.sun_family = AF_UNIX;      // 设置地址族为UNIX域套接字
    strcpy(sun.sun_path, name);    // 复制路径名到结构体

    // 计算sockaddr_un结构体的大小
    len = offsetof(struct sockaddr_un, sun_path) + strlen(name);

    // 尝试连接到服务器
    if (connect(fd, (struct sockaddr *)&sun, len) < 0)
    {
        rval      = -4;  // 设置错误码
        do_unlink = 1;   // 标记需要删除文件
        goto errout;     // 跳转到错误处理部分
    }

    // 成功连接后返回文件描述符
    return (fd);

errout:
    err = errno;  // 保存错误码
    close(fd);    // 关闭文件描述符
    if (do_unlink)
        unlink(un.sun_path);  // 如果需要,删除文件
    errno = err;              // 恢复错误码
    return (rval);            // 返回错误码
}
