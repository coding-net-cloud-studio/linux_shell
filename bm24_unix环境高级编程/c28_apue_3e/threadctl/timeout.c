#include "apue.h"      // 引入APUE库的头文件
#include <pthread.h>   // 引入POSIX线程库的头文件
#include <time.h>      // 引入时间函数库的头文件
#include <sys/time.h>  // 引入系统时间函数库的头文件

// 外部函数声明,用于创建新线程
// 参数:线程开始执行的函数指针和传递给该函数的参数
// 返回值:成功返回线程ID,失败返回错误码
extern int makethread(void *(*)(void *), void *);

// 定义一个结构体to_info,用于存储超时操作的相关信息
struct to_info
{
    void (*to_fn)(void *);    // 函数指针,指向要执行的函数
    void           *to_arg;   // 指向函数的参数
    struct timespec to_wait;  // 等待的时间
};

// 定义常量SECTONSEC,表示秒到纳秒的转换,1秒=1000000000纳秒
#define SECTONSEC 1000000000 /* seconds to nanoseconds */

// 如果没有定义CLOCK_REALTIME或者定义了BSD,则使用nanosleep函数代替clock_nanosleep
#if !defined(CLOCK_REALTIME) || defined(BSD)
#define clock_nanosleep(ID, FL, REQ, REM) nanosleep((REQ), (REM))
#endif

#ifndef CLOCK_REALTIME
#define CLOCK_REALTIME 0
#define USECTONSEC     1000 /* 微秒转换为纳秒的系数 */

/**
 * @brief 获取当前时间的时钟时间
 *
 * @param id 时钟类型ID,在这个实现中未使用
 * @param tsp 指向timespec结构体的指针,用于存储获取的时间
 */
void clock_gettime(int id, struct timespec *tsp)
{
    struct timeval tv;  // 定义一个timeval结构体用于存储从gettimeofday获取的时间

    gettimeofday(&tv, NULL);                 // 获取当前时间,存入tv
    tsp->tv_sec  = tv.tv_sec;                // 将秒数部分复制到tsp结构体
    tsp->tv_nsec = tv.tv_usec * USECTONSEC;  // 将微秒部分转换为纳秒,并复制到tsp结构体
}
#endif

void *
timeout_helper(void *arg)
{
    // 定义一个指向to_info结构体的指针
    struct to_info *tip;

    // 将传入的参数转换为to_info结构体指针
    tip = (struct to_info *)arg;

    // 使用clock_nanosleep函数使当前线程休眠指定的时间
    // CLOCK_REALTIME表示使用系统当前时间,0表示不设置剩余时间,&tip->to_wait是要休眠的时间
    clock_nanosleep(CLOCK_REALTIME, 0, &tip->to_wait, NULL);

    // 调用传入的函数指针,并传入相应的参数
    (*tip->to_fn)(tip->to_arg);

    // 释放之前分配的内存
    free(arg);

    // 返回NULL
    return (0);
}
void timeout(const struct timespec *when, void (*func)(void *), void *arg)
{
    // 获取当前时间
    // 使用标准库函数clock_gettime,它能够获取高精度的系统时间
    // 参数CLOCK_REALTIME表示获取的是相对于Epoch(1970年1月1日)的实时时间
    // 参数&now是一个timespec结构体的指针,用于存储获取到的时间
    struct timespec now;
    clock_gettime(CLOCK_REALTIME, &now);

    // 如果指定时间大于当前时间,则创建线程并延迟执行
    if ((when->tv_sec > now.tv_sec) ||
        (when->tv_sec == now.tv_sec && when->tv_nsec > now.tv_nsec))
    {
        // 分配内存用于存储定时器信息
        // 分配内存以存储一个to_info结构体实例
        // 使用sizeof运算符来确定所需内存的大小
        struct to_info *tip = malloc(sizeof(struct to_info));

        if (tip != NULL)
        {
            // 设置定时器信息
            // tip 是定时器结构体指针,func 是定时器到期时调用的函数,arg 是传递给该函数的参数
            // when 是定时器到期的时间点,now 是当前时间点
            tip->to_fn          = func;                       // 设置定时器到期时调用的函数
            tip->to_arg         = arg;                        // 设置传递给定时器函数的参数
            tip->to_wait.tv_sec = when->tv_sec - now.tv_sec;  // 计算定时器等待的秒数

            // 如果 when 的纳秒部分大于等于 now 的纳秒部分,直接计算差值
            // 否则,需要从秒数中借一位,并计算纳秒的差值
            // 当when的纳秒部分大于或等于now的纳秒部分时
            // 注释说明:该代码段用于计算两个时间点之间的差值,并将结果存储在tip结构体的to_wait成员中.
            if (when->tv_nsec >= now.tv_nsec)
            {
                // 计算tip的等待时间的纳秒部分,即when的纳秒部分减去now的纳秒部分
                tip->to_wait.tv_nsec = when->tv_nsec - now.tv_nsec;
            }
            else
            {
                tip->to_wait.tv_sec--;                                           // 秒数减一
                tip->to_wait.tv_nsec = SECTONSEC - now.tv_nsec + when->tv_nsec;  // 计算新的纳秒数
            }

            // 创建线程执行定时器辅助函数
            // timeout_helper 是定时器辅助函数,tip 是传递给该函数的参数
            int err = makethread(timeout_helper, (void *)tip);

            // 如果线程创建成功,直接返回
            // 如果线程创建失败,释放之前分配的内存
            if (err == 0)
                return;
            else
                free(tip);
        }
    }

    // 如果指定时间小于等于当前时间,或者内存分配失败,或者无法创建线程,则立即执行函数
    (*func)(arg);
}

// 定义互斥锁属性对象
pthread_mutexattr_t attr;
// 定义互斥锁对象
pthread_mutex_t mutex;

/**
 * @brief 重试函数,用于执行重试步骤
 * @param arg 函数参数,通常用于传递上下文信息
 */
void retry(void *arg)
{
    // 尝试获取互斥锁,如果锁被其他线程持有,则当前线程会被阻塞
    pthread_mutex_lock(&mutex);

    /* 执行重试步骤 ... */

    // 释放互斥锁,允许其他线程获取锁
    pthread_mutex_unlock(&mutex);
}
int main(void)
{
    int             err, condition, arg;  // 定义错误码,条件变量和参数
    struct timespec when;                 // 定义timespec结构体用于存储时间

    // 初始化互斥锁属性,如果失败则调用err_exit函数退出
    if ((err = pthread_mutexattr_init(&attr)) != 0)
        err_exit(err, "pthread_mutexattr_init failed");
    // 设置互斥锁属性为递归锁,如果失败则调用err_exit函数退出
    if ((err = pthread_mutexattr_settype(&attr,
                                         PTHREAD_MUTEX_RECURSIVE)) != 0)
        err_exit(err, "can't set recursive type");
    // 初始化互斥锁,如果失败则调用err_exit函数退出
    if ((err = pthread_mutex_init(&mutex, &attr)) != 0)
        err_exit(err, "can't create recursive mutex");

    /* continue processing ... */

    // 加锁
    // 加锁,确保互斥访问共享资源
    pthread_mutex_lock(&mutex);

    /*
     * 在锁的保护下检查条件,以使检查和调用timeout函数成为原子操作.
     */
    if (condition)
    {
        /*
         * 计算我们希望重试的绝对时间.
         */
        clock_gettime(CLOCK_REALTIME, &when);                 // 获取当前时间
        when.tv_sec += 10;                                    // 在当前时间基础上加10秒
        timeout(&when, retry, (void *)((unsigned long)arg));  // 调用timeout函数设置超时
    }
    pthread_mutex_unlock(&mutex);  // 解锁

    /* continue processing ... */

    exit(0);  // 程序正常退出
}
