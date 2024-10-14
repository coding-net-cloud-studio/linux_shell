#include "apue.h"
#include <sys/time.h>

// @brief 使当前进程休眠指定的微秒数
// @param nusecs 需要休眠的微秒数
void sleep_us(unsigned int nusecs)
{
    struct timeval tval;  // 定义一个timeval结构体用于存储时间值

    tval.tv_sec  = nusecs / 1000000;  // 计算需要休眠的秒数,将微秒转换为秒
    tval.tv_usec = nusecs % 1000000;  // 计算不足一秒部分的微秒数

    select(0, NULL, NULL, NULL, &tval);  // 使用select函数使进程休眠,前四个参数为无效文件描述符和空指针,最后一个参数为timeval结构体指针
}
