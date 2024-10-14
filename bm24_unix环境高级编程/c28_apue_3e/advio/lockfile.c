#include <unistd.h>  // 包含unistd.h头文件,提供对系统调用的访问
#include <fcntl.h>   // 包含fcntl.h头文件,提供对文件控制的访问


/**
 * @brief 对文件描述符fd进行加锁操作
 *
 * 该函数使用fcntl系统调用对文件描述符fd进行加写锁(独占锁).
 * 如果其他进程已经对该文件持有锁,则该调用会阻塞,直到锁被释放.
 *
 * @param fd 文件描述符,需要加锁的文件
 * @return 成功返回0,失败返回-1并设置errno
 */
int lockfile(int fd)
{
    struct flock fl;  // 定义一个flock结构体,用于存储锁的信息

    fl.l_type   = F_WRLCK;   // 设置锁的类型为F_WRLCK,即写锁(独占锁)
    fl.l_start  = 0;         // 锁的起始位置,从文件开头开始
    fl.l_whence = SEEK_SET;  // 锁的起始位置相对于文件的位置,SEEK_SET表示从文件开头
    fl.l_len    = 0;         // 锁的长度,0表示锁住整个文件

    // 使用fcntl系统调用设置文件锁
    // F_SETLK表示设置或清除文件锁,如果无法立即设置锁,则不会阻塞,而是立即返回错误
    return (fcntl(fd, F_SETLK, &fl));
}
