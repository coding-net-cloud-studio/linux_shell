#include <pthread.h>

// 定义消息结构体,包含指向下一个消息的指针
// 可能还有其他数据成员
struct msg
{
    struct msg *m_next;  // 指向下一个消息的指针
    /* ... more stuff here ... */
};

// 定义工作队列,用于存放消息结构体指针
struct msg *workq;

// 初始化条件变量qready,用于线程间的同步
// PTHREAD_COND_INITIALIZER是pthread库提供的宏,用于静态初始化条件变量
pthread_cond_t qready = PTHREAD_COND_INITIALIZER;

// 初始化互斥锁qlock,用于保护共享资源workq
// PTHREAD_MUTEX_INITIALIZER是pthread库提供的宏,用于静态初始化互斥锁
pthread_mutex_t qlock = PTHREAD_MUTEX_INITIALIZER;

// process_msg 函数用于处理消息队列中的消息.
// 该函数通过一个无限循环来检查消息队列,当队列为空时,线程会等待条件变量 qready 的通知.
// 一旦有消息可用,它会从队列中取出消息并处理.
void process_msg(void)
{
    // 定义一个指向 msg 结构体的指针 mp
    struct msg *mp;

    // 无限循环,持续处理消息
    for (;;)
    {
        // 加锁,保护消息队列的访问
        pthread_mutex_lock(&qlock);

        // 当工作队列为空时,线程在此等待条件变量 qready 的通知
        while (workq == NULL)
            pthread_cond_wait(&qready, &qlock);

        // 从队列中取出消息
        mp = workq;
        // 更新队列头指针,移除已取出的消息
        workq = mp->m_next;

        // 解锁,允许其他线程访问消息队列
        pthread_mutex_unlock(&qlock);

        // 现在处理消息 mp
    }
}

/**
 * @brief 将消息加入工作队列
 *
 * 该函数用于将一个消息节点加入到全局的工作队列中.它首先锁定互斥锁以防止并发访问,
 * 然后将新消息节点链接到工作队列的头部,并最后解锁互斥锁和发送条件变量信号通知等待的线程.
 *
 * @param mp 指向要加入队列的消息节点的指针
 */
void enqueue_msg(struct msg *mp)
{
    pthread_mutex_lock(&qlock);    // 锁定互斥锁,防止并发访问
    mp->m_next = workq;            // 将新消息节点链接到工作队列的头部
    workq      = mp;               // 更新工作队列的头指针
    pthread_mutex_unlock(&qlock);  // 解锁互斥锁
    pthread_cond_signal(&qready);  // 发送条件变量信号,通知等待的线程
}
