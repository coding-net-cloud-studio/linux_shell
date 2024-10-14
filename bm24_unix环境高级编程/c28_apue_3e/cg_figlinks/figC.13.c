#include <unistd.h>      // 包含unistd.h头文件,提供对POSIX操作系统API的访问
#include <time.h>        // 包含time.h头文件,提供日期和时间处理的函数
#include <sys/select.h>  // 包含sys/select.h头文件,提供select()函数,用于I/O多路复用

unsigned
sleep(unsigned seconds)  // 定义一个函数sleep,参数为无符号整型,返回值也为无符号整型
{
    int            n;           // 定义整型变量n,用于存储select函数的返回值
    unsigned       slept;       // 定义无符号整型变量slept,用于记录实际睡眠的时间
    time_t         start, end;  // 定义time_t类型的变量start和end,用于记录睡眠前后的时间
    struct timeval tv;          // 定义timeval结构体变量tv,用于设置select函数的超时时间

    tv.tv_sec  = seconds;                  // 设置tv的秒数为传入的秒数
    tv.tv_usec = 0;                        // 设置tv的微秒数为0
    time(&start);                          // 获取当前时间并赋值给start
    n = select(0, NULL, NULL, NULL, &tv);  // 调用select函数进行等待,超时时间为tv
    if (n == 0)                            // 如果select函数返回0,表示已经等待了足够的时间
        return (0);                        // 返回0表示睡眠完成
    time(&end);                            // 获取当前时间并赋值给end
    slept = end - start;                   // 计算实际睡眠的时间
    if (slept >= seconds)                  // 如果实际睡眠时间大于等于需要的睡眠时间
        return (0);                        // 返回0表示睡眠完成
    return (seconds - slept);              // 否则返回还需要睡眠的时间
}
