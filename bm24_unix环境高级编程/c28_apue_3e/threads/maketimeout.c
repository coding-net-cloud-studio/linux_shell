#include <sys/time.h>
#include <stdlib.h>

// @brief 计算并设置超时时间
// @param tsp 指向timespec结构体的指针,用于存储计算后的超时时间
// @param minutes 要添加的分钟数
void maketimeout(struct timespec *tsp, long minutes)
{
    struct timeval now;

    /* 获取当前时间 */
    gettimeofday(&now, NULL);
    tsp->tv_sec  = now.tv_sec;          // 将当前时间的秒数赋值给tsp的秒数部分
    tsp->tv_nsec = now.tv_usec * 1000;  // 将当前时间的微秒数转换为纳秒数,并赋值给tsp的纳秒数部分
    /* 添加分钟数偏移量以得到超时值 */
    tsp->tv_sec += minutes * 60;  // 将分钟数转换为秒数并加到tsp的秒数部分
}
