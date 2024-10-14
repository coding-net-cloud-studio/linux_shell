#include <stdlib.h>
#include <pthread.h>

// 定义工作结构体,包含指向下一个和上一个工作的指针,以及处理该工作的线程ID
struct job
{
    struct job *j_next;  // 指向下一个工作的指针
    struct job *j_prev;  // 指向前一个工作的指针
    pthread_t   j_id;    // 处理该工作的线程ID
                         // ... 更多内容 ...
};

// 定义队列结构体,包含指向队列头部和尾部的指针,以及用于保护队列的读写锁
struct queue
{
    struct job      *q_head;  // 指向队列头部的指针
    struct job      *q_tail;  // 指向队列尾部的指针
    pthread_rwlock_t q_lock;  // 保护队列的读写锁
};
/*
 * 初始化一个队列.
 *
 * @param qp 指向要初始化的队列结构的指针.
 * @return 如果初始化成功返回0,否则返回错误码.
 */
int queue_init(struct queue *qp)
{
    int err;

    // 初始化队列的头指针和尾指针为NULL
    qp->q_head = NULL;
    qp->q_tail = NULL;

    // 初始化读写锁
    err = pthread_rwlock_init(&qp->q_lock, NULL);
    if (err != 0)
        return (err);  // 如果初始化失败,返回错误码

    /* ... continue initialization ... */

    return (0);  // 初始化成功,返回0
}
/*
 * 在队列头部插入一个任务.
 *
 * @param qp 指向队列的指针
 * @param jp 指向要插入任务的指针
 */
void job_insert(struct queue *qp, struct job *jp)
{
    // 获取写锁,确保在修改队列时的线程安全
    pthread_rwlock_wrlock(&qp->q_lock);

    // 将新任务设置为队列头部
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
 * 在队列尾部追加一个任务.
 *
 * @param qp 指向队列的指针
 * @param jp 指向要追加的任务的指针
 *
 * 此函数使用读写锁来确保在多线程环境中安全地向队列追加任务.
 * 首先,它会获取写锁以独占访问队列.
 * 然后,它会将任务添加到队列的尾部.
 * 如果队列为空,新任务将成为队列的头节点.
 * 最后,释放写锁.
 */
void job_append(struct queue *qp, struct job *jp)
{
    pthread_rwlock_wrlock(&qp->q_lock);  // 获取写锁
    jp->j_next = NULL;                   // 新任务的 next 指针设为 NULL
    jp->j_prev = qp->q_tail;             // 新任务的 prev 指针指向当前队列尾部的任务
    if (qp->q_tail != NULL)              // 如果队列不为空
        qp->q_tail->j_next = jp;         // 当前队列尾部的任务的 next 指针指向新任务
    else                                 // 如果队列为空
        qp->q_head = jp;                 // 新任务成为队列的头节点
    qp->q_tail = jp;                     // 更新队列尾部的任务为新任务
    pthread_rwlock_unlock(&qp->q_lock);  // 释放写锁
}
/*
 * 从队列中移除指定的任务.
 *
 * @param qp 指向队列的指针.
 * @param jp 指向要移除的任务的指针.
 */
void job_remove(struct queue *qp, struct job *jp)
{
    // 获取写锁,确保在修改队列时的线程安全
    pthread_rwlock_wrlock(&qp->q_lock);

    // 如果要移除的任务是队列的头任务
    if (jp == qp->q_head)
    {
        qp->q_head = jp->j_next;  // 更新头任务为下一个任务

        // 如果要移除的任务也是尾任务,则更新尾任务为空
        if (qp->q_tail == jp)
            qp->q_tail = NULL;
        else
            // 否则,将原头任务的下一个任务的前驱设置为原头任务的前驱
            jp->j_next->j_prev = jp->j_prev;
    }
    // 如果要移除的任务是队列的尾任务
    else if (jp == qp->q_tail)
    {
        qp->q_tail         = jp->j_prev;  // 更新尾任务为前一个任务
        jp->j_prev->j_next = jp->j_next;  // 将原尾任务的前一个任务的后继设置为原尾任务的后继
    }
    else
    {
        // 如果要移除的任务既不是头任务也不是尾任务
        // 将其前驱任务的后继设置为其后继任务
        jp->j_prev->j_next = jp->j_next;
        // 将其后继任务的前驱设置为其前驱任务
        jp->j_next->j_prev = jp->j_prev;
    }

    // 释放写锁
    pthread_rwlock_unlock(&qp->q_lock);
}
/*
 * 查找给定线程ID的工作.
 *
 * @param qp 指向队列的指针,该队列包含工作项.
 * @param id 要查找的线程ID.
 * @return 如果找到匹配的工作项,则返回指向该工作项的指针;否则返回NULL.
 */
struct job *
job_find(struct queue *qp, pthread_t id)
{
    struct job *jp;  // 用于遍历工作项的指针

    // 尝试获取读锁,如果失败则返回NULL
    if (pthread_rwlock_rdlock(&qp->q_lock) != 0)
        return (NULL);

    // 遍历队列中的工作项
    for (jp = qp->q_head; jp != NULL; jp = jp->j_next)
        // 如果找到匹配的线程ID,则跳出循环
        if (pthread_equal(jp->j_id, id))
            break;

    // 释放读锁
    pthread_rwlock_unlock(&qp->q_lock);
    // 返回找到的工作项指针或NULL
    return (jp);
}
