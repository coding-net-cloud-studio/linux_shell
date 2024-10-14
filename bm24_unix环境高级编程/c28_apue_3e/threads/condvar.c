#include <pthread.h>

// 定义消息结构体,包含指向下一个消息的指针
struct msg
{
    struct msg *m_next;
    /* ... more stuff here ... */
};

// 定义全局消息队列指针
struct msg *workq;

// 初始化条件变量,用于线程间同步
// PTHREAD_COND_INITIALIZER 是一个宏,用于静态初始化条件变量
pthread_cond_t qready = PTHREAD_COND_INITIALIZER;

// 初始化互斥锁,用于保护共享资源
// PTHREAD_MUTEX_INITIALIZER 是一个宏,用于静态初始化互斥锁
pthread_mutex_t qlock = PTHREAD_MUTEX_INITIALIZER;

void process_msg(void)
{
    struct msg *mp;  // 定义一个指向msg结构体的指针mp

    for (;;)  // 无限循环,用于持续处理消息队列中的消息
    {
        pthread_mutex_lock(&qlock);              // 加锁,保证对共享资源workq的访问是线程安全的
        while (workq == NULL)                    // 如果工作队列为空,则等待
            pthread_cond_wait(&qready, &qlock);  // 等待条件变量qready被触发,同时释放锁qlock
        mp    = workq;                           // 从队列中取出一个消息
        workq = mp->m_next;                      // 更新队列头指针,指向下一个消息
        pthread_mutex_unlock(&qlock);            // 解锁,允许其他线程访问共享资源workq
        /* now process the message mp */         // 处理取出的消息mp
    }
}

// enqueue_msg 函数用于将消息添加到工作队列中,并通知等待的线程有新消息可用.
// 参数 mp 是指向要添加到队列中的消息结构体的指针.
void enqueue_msg(struct msg *mp)
{
    // 加锁保护工作队列,防止并发访问导致的数据竞争
    pthread_mutex_lock(&qlock);

    // 将新消息添加到工作队列的头部
    mp->m_next = workq;
    workq      = mp;

    // 解锁工作队列
    pthread_mutex_unlock(&qlock);

    // 发送信号通知等待队列的线程有新消息可用
    pthread_cond_signal(&qready);
}
