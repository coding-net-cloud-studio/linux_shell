#include "apue.h"   // 引入APUE库的头文件
#include <fcntl.h>  // 引入文件控制相关的头文件

/*
 * lock_reg - 对文件描述符fd进行加锁或解锁操作
 * @fd: 文件描述符
 * @cmd: 操作命令,如F_SETLK, F_SETLKW等
 * @type: 锁定类型,F_RDLCK(共享读锁),F_WRLCK(独占写锁),F_UNLCK(解锁)
 * @offset: 锁定的起始偏移量
 * @whence: 偏移量的起始位置,SEEK_SET(从文件开头),SEEK_CUR(从当前位置),SEEK_END(从文件末尾)
 * @len: 锁定的长度,0表示到文件末尾
 *
 * 返回值: 成功返回0,失败返回-1
 */
int lock_reg(int fd, int cmd, int type, off_t offset, int whence, off_t len)
{
    struct flock lock;  // 定义一个flock结构体用于存储锁的信息

    lock.l_type   = type;   /* 设置锁定类型 */
    lock.l_start  = offset; /* 设置锁定的起始偏移量 */
    lock.l_whence = whence; /* 设置偏移量的起始位置 */
    lock.l_len    = len;    /* 设置锁定的长度 */

    return (fcntl(fd, cmd, &lock));  // 调用fcntl函数进行加锁或解锁操作,并返回结果
}
