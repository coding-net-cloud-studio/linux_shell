#include "apue.h"        // 引入apue库头文件
#include <sys/socket.h>  // 引入系统socket头文件,用于网络通信
#include <sys/un.h>      // 引入UNIX域套接字头文件,用于本地进程间通信
#include <errno.h>       // 引入错误处理头文件

#define CLI_PATH "/var/tmp/"  // 定义客户端套接字文件路径
#define CLI_PERM S_IRWXU      // 定义客户端套接字文件权限,仅用户可读写执行

/*
 * 创建一个客户端端点并连接到服务器.
 * 如果一切正常,返回文件描述符,否则返回<0.
 */
int cli_conn(const char *name)
{
    int                fd, len, err, rval;  // 定义文件描述符,长度,错误码和返回值
    struct sockaddr_un un, sun;             // 定义UNIX域套接字地址结构
    int                do_unlink = 0;       // 标记是否需要删除文件

    // 检查名称长度是否超过结构体限制
    // 检查name字符串的长度是否大于等于un.sun_path的大小
    // 如果是,则设置errno为ENAMETOOLONG,表示文件名过长,并返回-1
    if (strlen(name) >= sizeof(un.sun_path))
    {
        errno = ENAMETOOLONG;  // 设置错误码
        return (-1);           // 返回错误代码
    }

    /* 创建一个UNIX域流套接字 */
    if ((fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
        return (-1);

    /* 填充套接字地址结构体 */
    // 初始化一个sockaddr_un结构体变量un,并将其清零
    memset(&un, 0, sizeof(un));

    // 设置地址族为AF_UNIX,表示使用Unix域套接字
    un.sun_family = AF_UNIX;

    // 格式化字符串,生成一个唯一的Unix域套接字路径
    // CLI_PATH是一个预定义的路径前缀,getpid()获取当前进程ID
    sprintf(un.sun_path, "%s%05ld", CLI_PATH, (long)getpid());

    // 打印生成的Unix域套接字路径
    printf("file is %s\n", un.sun_path);

    // 计算sockaddr_un结构体的长度,包括sun_path字段的长度
    len = offsetof(struct sockaddr_un, sun_path) + strlen(un.sun_path);

    // 如果文件已存在,则删除
    // 删除之前创建的Unix域套接字文件
    unlink(un.sun_path);

    // 尝试将套接字绑定到指定的地址
    if (bind(fd, (struct sockaddr *)&un, len) < 0)
    {
        rval = -2;    // 绑定失败,设置错误码
        goto errout;  // 跳转到错误处理部分
    }

    // 尝试更改Unix域套接字文件的权限
    if (chmod(un.sun_path, CLI_PERM) < 0)
    {
        rval      = -3;  // 更改权限失败,设置错误码
        do_unlink = 1;   // 标记需要删除套接字文件
        goto errout;     // 跳转到错误处理部分
    }

    /* 填充服务器地址结构体 */
    // 初始化一个sockaddr_un结构体变量sun,并将其清零
    memset(&sun, 0, sizeof(sun));
    // 设置地址族为UNIX域协议
    sun.sun_family = AF_UNIX;
    // 将传入的name字符串复制到sun结构体的sun_path字段
    strcpy(sun.sun_path, name);
    // 计算sockaddr_un结构体的长度,包括sun_path字段的长度
    len = offsetof(struct sockaddr_un, sun_path) + strlen(name);
    // 尝试通过文件描述符fd连接到指定的UNIX域套接字
    if (connect(fd, (struct sockaddr *)&sun, len) < 0)
    {
        // 如果连接失败,设置错误码并标记需要删除套接字文件
        rval      = -4;
        do_unlink = 1;
        goto errout;  // 跳转到错误处理部分
    }
    // 如果连接成功,返回文件描述符
    return (fd);

errout:
    err = errno;  // 保存错误码
    close(fd);    // 关闭文件描述符
    if (do_unlink)
        unlink(un.sun_path);  // 删除文件
    errno = err;              // 恢复错误码
    return (rval);            // 返回错误码
}
