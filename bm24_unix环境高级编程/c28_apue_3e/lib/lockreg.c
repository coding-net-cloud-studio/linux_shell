#include "apue.h"   // 引入APUE库的头文件,提供系统调用和标准C库函数的封装
#include <fcntl.h>  // 引入文件控制操作的头文件,如open(), fcntl()等

/**
 * @brief 对文件描述符进行加锁或解锁操作
 *
 * @param fd 文件描述符
 * @param cmd 操作命令,F_SETLK用于设置锁,F_GETLK用于测试锁
 * @param type 锁的类型,F_RDLCK共享读锁,F_WRLCK独占写锁,F_UNLCK解锁
 * @param offset 锁定的起始字节偏移量
 * @param whence 偏移量的起始位置,SEEK_SET从文件开始,SEEK_CUR从当前位置,SEEK_END从文件末尾
 * @param len 锁定的字节数,0表示锁定到文件末尾
 * @return int 成功返回0,失败返回-1
 */
int lock_reg(int fd, int cmd, int type, off_t offset, int whence, off_t len)
{
    struct flock lock;  // 定义一个flock结构体用于存储锁的信息

    lock.l_type   = type;    // 设置锁的类型
    lock.l_start  = offset;  // 设置锁的起始位置
    lock.l_whence = whence;  // 设置偏移量的起始位置
    lock.l_len    = len;     // 设置锁定的长度

    return (fcntl(fd, cmd, &lock));  // 调用fcntl函数执行加锁或解锁操作,并返回结果
}
