#include "apue.h"        // 引入apue库的头文件
#include <sys/socket.h>  // 引入socket编程所需的头文件
#include <sys/un.h>      // 引入UNIX域套接字所需的头文件
#include <time.h>        // 引入时间函数所需的头文件
#include <errno.h>       // 引入错误处理所需的头文件

#define STALE 30  // 定义客户端名称的最大过期时间(秒)
/*
 * 等待客户端连接到达,并接受它.
 * 我们还从它在调用我们之前必须绑定的路径名中获取客户端的用户ID.
 * 如果一切正常,返回新的文件描述符,如果出错,返回<0
 */
int serv_accept(int listenfd, uid_t *uidptr)
{
    int                clifd, err, rval;  // clifd: 客户端文件描述符, err: 错误代码, rval: 返回值
    socklen_t          len;               // len: 地址结构体长度
    time_t             staletime;         // staletime: 过去的时间
    struct sockaddr_un un;                // un: UNIX域套接字地址结构
    struct stat        statbuf;           // statbuf: 文件状态结构
    char              *name;              // name: 客户端路径名

    /* 分配足够的空间用于最长的名称加上终止的空字符 */
    // 为name分配内存空间,大小为un.sun_path的长度加1
    if ((name = malloc(sizeof(un.sun_path + 1))) == NULL)
        return (-1);  // 如果内存分配失败,返回-1

    len = sizeof(un);  // 设置len为un结构体的大小

    // 接受一个连接请求,将客户端信息存储在un结构体中,并通过len返回实际接收到的信息长度
    if ((clifd = accept(listenfd, (struct sockaddr *)&un, &len)) < 0)
    {
        free(name);   // 如果accept失败,释放之前分配给name的内存
        return (-2);  // 返回-2,通常表示errno=EINTR,即捕获到信号
    }

    /* 从客户端的调用地址获取客户端的uid */
    // 从结构体sockaddr_un中减去sun_path字段的偏移量,得到路径名的实际长度
    len -= offsetof(struct sockaddr_un, sun_path);

    // 将路径名复制到name数组中
    memcpy(name, un.sun_path, len);

    // 在路径名末尾添加空字符,以确保字符串正确终止
    name[len] = 0;

    // 使用stat函数检查路径名对应的文件状态
    // 如果stat函数返回值小于0,表示发生了错误
    if (stat(name, &statbuf) < 0)
    {
        // 设置错误码并跳转到错误处理标签
        rval = -3;
        goto errout;
    }

#ifdef S_ISSOCK /* SVR4未定义 */
    // 检查statbuf.st_mode是否表示一个套接字文件
    if (S_ISSOCK(statbuf.st_mode) == 0)
    {
        rval = -4;  // 如果不是套接字,则设置错误码并跳转到错误处理部分
        goto errout;
    }

#endif

    // 检查文件权限是否为rwx------
    // statbuf.st_mode & (S_IRWXG | S_IRWXO) 检查是否有组或其他用户的读写执行权限
    // statbuf.st_mode & S_IRWXU != S_IRWXU 检查是否所有用户(拥有者)的权限都是读写执行
    // 如果上述任一条件为真,则表示权限不符合rwx------的要求
    if ((statbuf.st_mode & (S_IRWXG | S_IRWXO)) ||
        (statbuf.st_mode & S_IRWXU) != S_IRWXU)
    {
        rval = -5;    // 设置返回值为-5,表示权限不符合要求
        goto errout;  // 跳转到错误处理部分
    }

    // 检查文件的访问时间,更改时间和修改时间是否都比当前时间早STALE秒
    // 如果任何一个时间早于当前时间减去STALE秒,则认为文件i节点太旧,返回错误码-6
    staletime = time(NULL) - STALE;      // 计算当前时间减去STALE秒的时间点
    if (statbuf.st_atime < staletime ||  // 检查访问时间
        statbuf.st_ctime < staletime ||  // 检查更改时间
        statbuf.st_mtime < staletime)    // 检查修改时间
    {
        rval = -6;    // 设置错误码为-6,表示i节点太旧
        goto errout;  // 跳转到错误处理部分
    }

    // 如果uidptr不为空,则将statbuf.st_uid的值赋给uidptr指向的内存地址,即返回调用者的用户ID
    if (uidptr != NULL)
        *uidptr = statbuf.st_uid; /* 返回调用者的uid */

    // 删除之前创建的路径名,因为我们已经完成了对它的使用
    unlink(name); /* 我们现在完成了路径名 */

    // 释放之前分配的内存空间
    free(name);

    // 返回文件描述符clifd
    return (clifd);

// errout: 错误处理标签
// 当发生错误时,跳转到此标签进行处理
errout:
    // 保存当前errno值,以便后续恢复
    err = errno;
    // 关闭客户端文件描述符
    close(clifd);
    // 释放name指针指向的内存
    free(name);
    // 恢复之前保存的errno值
    errno = err;
    // 返回函数结果值
    return (rval);
}
