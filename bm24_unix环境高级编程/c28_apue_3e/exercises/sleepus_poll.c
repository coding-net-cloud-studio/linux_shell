#include <poll.h>

// @brief 使当前线程休眠指定的微秒数
//
// 该函数通过调用poll函数实现非阻塞的休眠.poll函数通常用于I/O多路复用,
// 但在这里我们只使用它的超时功能.我们将一个无效的文件描述符结构体传递给poll,
// 并设置超时时间,从而实现休眠效果.
//
// @param nusecs 要休眠的微秒数
void sleep_us(unsigned int nusecs)
{
    struct pollfd dummy;    // 创建一个无效的pollfd结构体
    int           timeout;  // 计算poll函数的超时时间(毫秒)

    // 如果nusecs小于等于0,则将timeout设置为1毫秒,确保至少休眠最小单位
    if ((timeout = nusecs / 1000) <= 0)
        timeout = 1;
    poll(&dummy, 0, timeout);  // 调用poll函数,实现休眠
}
