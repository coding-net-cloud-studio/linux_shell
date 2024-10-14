#include <unistd.h>
#include <time.h>
#include <sys/select.h>

/**
 * @brief 使当前进程休眠指定的秒数
 *
 * @param seconds 需要休眠的秒数
 * @return unsigned 实际休眠的剩余秒数,如果完全休眠则返回0
 */
unsigned
sleep(unsigned seconds)
{
    int            n;           // select函数的返回值
    unsigned       slept;       // 实际休眠的时间(秒)
    time_t         start, end;  // 开始和结束时间
    struct timeval tv;          // select函数使用的超时结构体

    tv.tv_sec  = seconds;  // 设置超时时间为指定的秒数
    tv.tv_usec = 0;        // 微秒部分设置为0

    time(&start);                          // 记录开始时间
    n = select(0, NULL, NULL, NULL, &tv);  // 使用select函数进行休眠
    if (n == 0)                            // 如果select函数返回0,表示已经休眠了指定的时间
        return (0);
    time(&end);            // 记录结束时间
    slept = end - start;   // 计算实际休眠的时间
    if (slept >= seconds)  // 如果实际休眠时间大于等于指定时间,返回0
        return (0);
    return (seconds - slept);  // 否则返回剩余未休眠的时间
}
