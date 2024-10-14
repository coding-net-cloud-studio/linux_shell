#include <poll.h>  // 引入poll函数的头文件,用于I/O多路复用

/*
 * 函数名:sleep_us
 * 功能:使当前线程暂停指定的微秒数
 * 参数:
 *   nusecs - 需要暂停的微秒数
 * 返回值:无
 *
 * 该函数通过调用poll函数实现延迟,poll函数会阻塞直到超时或者有事件发生.
 * 由于poll函数的第三个参数是以毫秒为单位的,因此需要将微秒转换为毫秒.
 * 如果转换后的毫秒数小于等于0,则设置超时时间为1毫秒,以确保至少有短暂的延迟.
 */
void sleep_us(unsigned int nusecs)
{
    struct pollfd dummy;    // 创建一个pollfd结构体实例,但不用于实际的事件监听
    int           timeout;  // 定义超时时间

    if ((timeout = nusecs / 1000) <= 0)  // 将微秒转换为毫秒
        timeout = 1;                     // 如果转换后的毫秒数小于等于0,则设置超时时间为1毫秒
    poll(&dummy, 0, timeout);            // 调用poll函数,传入dummy结构体指针,监听事件数量为0,超时时间为timeout毫秒
}
