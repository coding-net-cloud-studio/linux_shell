#include <unistd.h>  // 包含unistd.h头文件,提供对POSIX操作系统API的访问
#include <fcntl.h>   // 包含fcntl.h头文件,提供对文件控制操作的支持

// lockfile 函数用于对文件描述符 fd 进行加锁
// 使用 F_WRLCK 类型的锁,表示写锁
// l_start 设置为 0,表示从文件开始位置加锁
// l_whence 设置为 SEEK_SET,表示相对文件起始位置
// l_len 设置为 0,表示加锁整个文件
// 返回值是 fcntl 系统调用的返回值,成功则返回 0
int lockfile(int fd)
{
    struct flock fl;

    fl.l_type   = F_WRLCK;   // 设置锁类型为写锁
    fl.l_start  = 0;         // 锁定区域的开始偏移量为 0
    fl.l_whence = SEEK_SET;  // 偏移量的起点为文件开头
    fl.l_len    = 0;         // 锁定整个文件,长度为 0 表示到文件末尾

    return (fcntl(fd, F_SETLK, &fl));  // 调用 fcntl 函数进行加锁操作
}
