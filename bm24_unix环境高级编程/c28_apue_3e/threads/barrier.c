#include "apue.h"      // 引入APUE库的头文件
#include <pthread.h>   // 引入POSIX线程库的头文件
#include <limits.h>    // 引入定义各种数据类型限制的头文件
#include <sys/time.h>  // 引入定义时间和日期函数的系统头文件

#define NTHR   8                // 定义线程数量为8
#define NUMNUM 8000000L         // 定义要排序的数字总数为800万
#define TNUM   (NUMNUM / NTHR)  // 定义每个线程需要排序的数字数量

long nums[NUMNUM];   // 定义一个长整型数组,用于存储待排序的数字
long snums[NUMNUM];  // 定义一个长整型数组,用于存储排序后的数字

pthread_barrier_t b;  // 定义一个线程屏障,用于同步多个线程

#ifdef SOLARIS
#define heapsort qsort  // 如果是在SOLARIS系统上,将heapsort定义为qsort
#else
extern int heapsort(void *, size_t, size_t, int (*)(const void *, const void *));  // 声明heapsort函数,用于堆排序
#endif

/*
 * 比较两个长整型整数(堆排序的辅助函数)
 *
 * @param arg1 第一个长整型整数的指针
 * @param arg2 第二个长整型整数的指针
 * @return 如果l1等于l2返回0;如果l1小于l2返回-1;如果l1大于l2返回1
 */
int complong(const void *arg1, const void *arg2)
{
    long l1 = *(long *)arg1;  // 将arg1转换为长整型指针并解引用得到l1
    long l2 = *(long *)arg2;  // 将arg2转换为长整型指针并解引用得到l2

    if (l1 == l2)      // 如果l1等于l2
        return 0;      // 返回0
    else if (l1 < l2)  // 如果l1小于l2
        return -1;     // 返回-1
    else               // 否则,即l1大于l2
        return 1;      // 返回1
}

/*
 * 工作线程用于对一组数字的一部分进行排序.
 */
void *
thr_fn(void *arg)
{
    // 将传入的参数转换为长整型索引
    long idx = (long)arg;

    // 使用堆排序算法对从idx开始的数字进行排序
    // nums是全局数组,TNUM是排序的数量,sizeof(long)是每个元素的大小,complong是比较函数
    heapsort(&nums[idx], TNUM, sizeof(long), complong);

    // 等待所有线程完成排序,这是通过屏障实现的
    pthread_barrier_wait(&b);

    /*
     * 继续执行更多的工作...
     */
    return ((void *)0);  // 返回空指针表示线程正常结束
}
/*
 * 合并各个排序范围的结果.
 */
void merge()
{
    long idx[NTHR];             // 存储每个线程当前处理的元素索引
    long i, minidx, sidx, num;  // i用于循环,minidx存储最小元素的索引,sidx存储合并后数组的索引,num存储当前找到的最小值

    // 初始化每个线程处理的元素索引
    for (i = 0; i < NTHR; i++)
        idx[i] = i * TNUM;
    // 遍历所有需要合并的元素
    for (sidx = 0; sidx < NUMNUM; sidx++)
    {
        num = LONG_MAX;  // 初始化num为最大长整型值
        // 寻找当前所有线程处理的最小元素
        for (i = 0; i < NTHR; i++)
        {
            // 如果当前线程还有元素未处理,并且该元素小于当前找到的最小值
            if ((idx[i] < (i + 1) * TNUM) && (nums[idx[i]] < num))
            {
                num    = nums[idx[i]];  // 更新最小值
                minidx = i;             // 更新最小值的线程索引
            }
        }
        snums[sidx] = nums[idx[minidx]];  // 将最小值放入合并后的数组
        idx[minidx]++;                    // 移动该线程处理的元素索引到下一个
    }
}
int main()
{
    // 定义无符号长整型变量i,用于循环等操作
    unsigned long i;
    // 定义结构体timeval变量start和end,用于记录开始和结束时间
    struct timeval start, end;
    // 定义长整型变量startusec和endusec,用于存储开始和结束时间的微秒部分
    long long startusec, endusec;
    // 定义双精度浮点型变量elapsed,用于计算排序所花费的时间
    double elapsed;
    // 定义整型变量err,用于存储线程创建等操作的错误码
    int err;
    // 定义pthread_t类型的变量tid,用于存储线程ID
    pthread_t tid;

    /*
     * 创建初始的一组待排序数字.
     */
    srandom(1);                   // 设置随机数种子
    for (i = 0; i < NUMNUM; i++)  // 循环生成NUMNUM个随机数
        nums[i] = random();       // 将生成的随机数存入nums数组

    /*
     * 创建8个线程来排序这些数字.
     */
    gettimeofday(&start, NULL);                // 获取当前时间作为开始时间
    pthread_barrier_init(&b, NULL, NTHR + 1);  // 初始化线程屏障
    for (i = 0; i < NTHR; i++)                 // 循环创建NTHR个线程
    {
        err = pthread_create(&tid, NULL, thr_fn, (void *)(i * TNUM));  // 创建线程
        if (err != 0)                                                  // 如果线程创建失败
            err_exit(err, "can't create thread");                      // 输出错误信息并退出程序
    }
    pthread_barrier_wait(&b);  // 等待所有线程完成排序
    merge();                   // 合并各个线程排序的结果
    gettimeofday(&end, NULL);  // 获取当前时间作为结束时间

    /*
     * 打印排序后的列表.
     */
    startusec = start.tv_sec * 1000000 + start.tv_usec;     // 将开始时间的秒和微秒部分转换为微秒
    endusec   = end.tv_sec * 1000000 + end.tv_usec;         // 将结束时间的秒和微秒部分转换为微秒
    elapsed   = (double)(endusec - startusec) / 1000000.0;  // 计算排序所花费的时间(秒)
    printf("sort took %.4f seconds\n", elapsed);            // 输出排序所花费的时间
    for (i = 0; i < NUMNUM; i++)                            // 循环打印排序后的数字
        printf("%ld\n", snums[i]);
    exit(0);  // 退出程序
}
