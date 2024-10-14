#include "apue.h"
#include <sys/select.h>

// @brief 使当前线程休眠指定的微秒数
// @param nusecs 要休眠的微秒数
void sleep_us(unsigned int nusecs)
{
    struct timeval tval;  // 定义一个timeval结构体用于select函数

    tval.tv_sec  = nusecs / 1000000;     // 计算需要休眠的秒数
    tval.tv_usec = nusecs % 1000000;     // 计算剩余的微秒数
    select(0, NULL, NULL, NULL, &tval);  // 调用select函数使线程休眠
}
