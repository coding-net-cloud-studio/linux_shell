#include <stdlib.h>   // 引入标准库,提供内存分配等功能
#include <pthread.h>  // 引入POSIX线程库,提供多线程编程支持

// 定义一个作业结构体
struct job
{
    struct job *j_next;  // 指向下一个作业的指针
    struct job *j_prev;  // 指向前一个作业的指针
    pthread_t   j_id;    // 标识处理此作业的线程ID
                         // ... 这里还有更多内容 ...
};

// 定义一个队列结构体
struct queue
{
    // 队列头指针,指向队列中的第一个元素
    struct job *q_head;

    // 队列尾指针,指向队列中的最后一个元素
    struct job *q_tail;

    // 读写锁,用于保护队列的并发访问
    pthread_rwlock_t q_lock;
};

/*
 * 初始化一个队列.
 *
 * @param qp 指向队列结构体的指针.
 * @return 如果初始化成功返回0,否则返回错误码.
 */
int queue_init(struct queue *qp)
{
    int err;

    // 初始化队列头和尾指针为NULL
    qp->q_head = NULL;
    qp->q_tail = NULL;

    // 初始化读写锁
    err = pthread_rwlock_init(&qp->q_lock, NULL);
    if (err != 0)
        return (err);

    /* ... 继续初始化 ... */

    // 初始化成功返回0
    return (0);
}

/*
 * 在队列头部插入一个任务.
 *
 * @param qp 指向队列的指针
 * @param jp 指向要插入的任务的指针
 */
void job_insert(struct queue *qp, struct job *jp)
{
    // 获取写锁,确保线程安全
    pthread_rwlock_wrlock(&qp->q_lock);

    // 将任务插入到队列头部
    jp->j_next = qp->q_head;
    jp->j_prev = NULL;

    // 如果队列不为空,更新原队列头部的前驱指针
    if (qp->q_head != NULL)
        qp->q_head->j_prev = jp;
    else
        // 如果队列为空,新任务同时成为队列尾部
        qp->q_tail = jp;

    // 更新队列头部为新任务
    qp->q_head = jp;

    // 释放写锁
    pthread_rwlock_unlock(&qp->q_lock);
}

/*
 * 将一个作业追加到队列的尾部.
 *
 * @param qp 指向队列的指针,该队列将接收新的作业.
 * @param jp 指向要追加的作业的指针.
 */
void job_append(struct queue *qp, struct job *jp)
{
    // 获取写锁,以确保在修改队列时的线程安全
    pthread_rwlock_wrlock(&qp->q_lock);

    // 新作业的下一个指针设置为NULL,因为它将是队列的最后一个元素
    jp->j_next = NULL;

    // 新作业的前一个指针设置为当前队列的尾部
    jp->j_prev = qp->q_tail;

    // 如果队列不为空,则将当前尾部作业的下一个指针指向新作业
    if (qp->q_tail != NULL)
        qp->q_tail->j_next = jp;
    else
        // 如果队列为空,则新作业也是头部作业
        qp->q_head = jp;

    // 更新队列的尾部指针为新作业
    qp->q_tail = jp;

    // 释放写锁
    pthread_rwlock_unlock(&qp->q_lock);
}

/*
 * 从队列中移除指定的作业.
 *
 * @param qp 指向队列的指针.
 * @param jp 指向要移除的作业的指针.
 */
void job_remove(struct queue *qp, struct job *jp)
{
    // 获取队列的写锁
    pthread_rwlock_wrlock(&qp->q_lock);

    // 如果作业是队列的头作业
    if (jp == qp->q_head)
    {
        qp->q_head = jp->j_next;  // 更新头作业为下一个作业
        // 如果作业同时是尾作业
        if (qp->q_tail == jp)
            qp->q_tail = NULL;  // 更新尾作业为空
        else
            jp->j_next->j_prev = jp->j_prev;  // 更新下一个作业的前一个作业
    }
    // 如果作业是队列的尾作业
    else if (jp == qp->q_tail)
    {
        qp->q_tail         = jp->j_prev;  // 更新尾作业为前一个作业
        jp->j_prev->j_next = jp->j_next;  // 更新前一个作业的下一个作业
    }
    else
    {
        // 如果作业既不是头作业也不是尾作业
        jp->j_prev->j_next = jp->j_next;  // 更新前一个作业的下一个作业
        jp->j_next->j_prev = jp->j_prev;  // 更新下一个作业的前一个作业
    }

    // 释放队列的写锁
    pthread_rwlock_unlock(&qp->q_lock);
}

/*
 * 查找给定线程ID的工作.
 * @param qp 指向队列的指针,该队列包含工作项.
 * @param id 要查找的线程ID.
 * @return 如果找到匹配的工作项,则返回指向该工作项的指针;否则返回NULL.
 */
struct job *
job_find(struct queue *qp, pthread_t id)
{
    struct job *jp;  // 用于遍历工作项的指针

    // 尝试获取队列的读锁
    if (pthread_rwlock_rdlock(&qp->q_lock) != 0)
        return (NULL);  // 如果获取锁失败,返回NULL

    // 遍历队列中的工作项
    for (jp = qp->q_head; jp != NULL; jp = jp->j_next)
        // 如果找到匹配的线程ID,则跳出循环
        if (pthread_equal(jp->j_id, id))
            break;

    pthread_rwlock_unlock(&qp->q_lock);  // 释放读锁
    return (jp);                         // 返回找到的工作项指针,如果没有找到则返回NULL
}
