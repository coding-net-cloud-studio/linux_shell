/* 创建一个命名流管道.由服务器在初始化时调用. */

#include "apue.h"
#include <sys/socket.h>
#include <sys/un.h>

/**
 * @brief 创建一个命名流管道
 * @param name 管道的名称
 * @param fd 一个包含两个文件描述符的数组,fd[0]用于读取,fd[1]用于写入
 * @return 如果一切正常返回0,如果发生错误返回-1(errno会被设置)
 */
int /* returns 0 if all OK, -1 if error (with errno set) */
ns_pipe(const char *name, int fd[2])
{
    int                len;        // 用于存储地址长度
    struct sockaddr_un unix_addr;  // UNIX域套接字地址结构

    if (fd_pipe(fd) < 0) /* 创建未命名的流管道 */
        return (-1);     // 如果创建失败,返回-1

    unlink(name); /* 如果名称已存在,则移除 */

    memset(&unix_addr, 0, sizeof(unix_addr));                         // 清空地址结构
    unix_addr.sun_family = AF_UNIX;                                   // 设置地址族为UNIX域
    strcpy(unix_addr.sun_path, name);                                 // 复制管道名称到地址结构
    len = strlen(unix_addr.sun_path) + sizeof(unix_addr.sun_family);  // 计算地址长度

    return (bind(fd[0], (struct sockaddr *)&unix_addr, len));  // 绑定地址到读文件描述符
    /* fd[0]已经绑定了名称 */
}
