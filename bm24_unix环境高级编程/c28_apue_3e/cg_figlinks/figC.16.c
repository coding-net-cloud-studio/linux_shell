#include "apue.h"        // 引入apue库,提供系统编程接口
#include <sys/select.h>  // 引入select函数的头文件,用于监视多个文件描述符

/*
 * 函数功能:使当前线程暂停指定的微秒数
 * 参数:
 *   nusecs - 需要暂停的微秒数
 */
void sleep_us(unsigned int nusecs)
{
    // 定义一个timeval结构体,用于存储时间值
    struct timeval tval;

    // 计算需要暂停的秒数,nusecs除以1000000得到秒数
    tval.tv_sec = nusecs / 1000000;
    // 计算剩余的微秒数,nusecs除以1000000的余数
    tval.tv_usec = nusecs % 1000000;
    // 使用select函数使当前线程暂停,直到指定的时间过去
    select(0, NULL, NULL, NULL, &tval);
}
