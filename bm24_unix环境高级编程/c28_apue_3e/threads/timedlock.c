#include "apue.h"
#include <pthread.h>

int main(void)
{
    int             err;                               // 错误码
    struct timespec tout;                              // 定义一个timespec结构体用于存储时间
    struct tm      *tmp;                               // 定义一个tm结构体指针用于存储时间的各个部分
    char            buf[64];                           // 定义一个字符数组用于格式化输出时间
    pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;  // 初始化一个互斥锁

    pthread_mutex_lock(&lock);                                  // 尝试锁定互斥锁
    printf("mutex is locked\n");                                // 如果成功,打印互斥锁已锁定
    clock_gettime(CLOCK_REALTIME, &tout);                       // 获取当前时间
    tmp = localtime(&tout.tv_sec);                              // 将时间转换为本地时间
    strftime(buf, sizeof(buf), "%r", tmp);                      // 格式化时间为字符串
    printf("current time is %s\n", buf);                        // 打印当前时间
    tout.tv_sec += 10;                                          // 在当前时间基础上加10秒
    /* caution: this could lead to deadlock */                  // 注意:这可能会导致死锁
    err = pthread_mutex_timedlock(&lock, &tout);                // 尝试在指定时间后锁定互斥锁
    clock_gettime(CLOCK_REALTIME, &tout);                       // 再次获取当前时间
    tmp = localtime(&tout.tv_sec);                              // 将时间转换为本地时间
    strftime(buf, sizeof(buf), "%r", tmp);                      // 格式化时间为字符串
    printf("the time is now %s\n", buf);                        // 打印当前时间
    if (err == 0)                                               // 如果互斥锁成功锁定
        printf("mutex locked again!\n");                        // 打印互斥锁再次锁定
    else                                                        // 如果互斥锁未能锁定
        printf("can't lock mutex again: %s\n", strerror(err));  // 打印错误信息
    exit(0);                                                    // 程序正常退出
}
