#include "apue.h"   // 引入APUE库的头文件,该库提供了一套用于UNIX和Linux环境下的系统编程接口
#include <fcntl.h>  // 引入文件控制相关的头文件,包含了文件打开,关闭,锁定等操作的函数原型

// @brief 测试文件锁定的函数
// @param fd 文件描述符
// @param type 锁定类型,F_RDLCK 表示共享锁定,F_WRLCK 表示独占锁定
// @param offset 锁定的起始偏移量
// @param whence 偏移量的起始位置,SEEK_SET 表示从文件开始,SEEK_CUR 表示从当前位置,SEEK_END 表示从文件末尾
// @param len 锁定的长度,0 表示锁定到文件末尾
// @return 如果区域未被其他进程锁定,返回0;如果被锁定,返回锁定进程的PID
pid_t lock_test(int fd, int type, off_t offset, int whence, off_t len)
{
    struct flock lock;  // 定义一个flock结构体用于存储锁定信息

    lock.l_type   = type;    // 设置锁定类型
    lock.l_start  = offset;  // 设置锁定起始偏移量
    lock.l_whence = whence;  // 设置偏移量的起始位置
    lock.l_len    = len;     // 设置锁定长度

    if (fcntl(fd, F_GETLK, &lock) < 0)  // 使用fcntl函数尝试获取文件锁定的状态
        err_sys("fcntl error");         // 如果出错,调用err_sys函数报告错误

    if (lock.l_type == F_UNLCK)  // 如果锁定类型为F_UNLCK,表示区域未被锁定
        return (0);              // 返回0,表示区域未被其他进程锁定
    return (lock.l_pid);         // 如果区域被锁定,返回锁定进程的PID
}
