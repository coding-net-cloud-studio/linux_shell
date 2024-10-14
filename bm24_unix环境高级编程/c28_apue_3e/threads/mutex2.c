#include <stdlib.h>
#include <pthread.h>

// 定义哈希表的大小
#define NHASH    29

// 定义一个简单的哈希函数,用于计算ID的哈希值
#define HASH(id) (((unsigned long)id) % NHASH)

// 定义一个结构体指针数组,用于存储哈希表的桶
struct foo *fh[NHASH];

// 初始化一个互斥锁,用于保护哈希表的并发访问
pthread_mutex_t hashlock = PTHREAD_MUTEX_INITIALIZER;

struct foo
{
    int             f_count;  // 记录当前结构体实例被引用的次数
    pthread_mutex_t f_lock;   // 用于保护结构体的互斥锁
    int             f_id;     // 结构体的唯一标识符
    struct foo     *f_next;   /* 受 hashlock 保护的链表中的下一个元素 */
                              /* ... more stuff here ... */
};

// foo_alloc 函数用于分配一个新的 foo 结构体实例,并进行初始化.
// 参数:
//   id: 要分配的 foo 实例的唯一标识符.
//
// 返回值:
//   成功时返回指向新分配的 foo 实例的指针,失败时返回 NULL.
struct foo *
foo_alloc(int id) /* allocate the object */
{
    struct foo *fp;   // 指向新分配的 foo 实例的指针
    int         idx;  // 用于散列函数的索引

    // 分配内存空间给新的 foo 实例
    if ((fp = malloc(sizeof(struct foo))) != NULL)
    {
        fp->f_count = 1;   // 初始化引用计数为 1
        fp->f_id    = id;  // 设置 foo 实例的 ID

        // 初始化 foo 实例的互斥锁
        if (pthread_mutex_init(&fp->f_lock, NULL) != 0)
        {
            free(fp);  // 如果互斥锁初始化失败,释放已分配的内存
            return (NULL);
        }

        idx = HASH(id);  // 计算 ID 的散列值

        // 加锁全局散列表,以安全地将新的 foo 实例插入到散列表中
        pthread_mutex_lock(&hashlock);
        fp->f_next = fh[idx];             // 将新的 foo 实例链接到散列表的相应位置
        fh[idx]    = fp;                  // 更新散列表的头指针
        pthread_mutex_unlock(&hashlock);  // 解锁全局散列表

        // 锁定新分配的 foo 实例的互斥锁,以便安全地继续初始化
        pthread_mutex_lock(&fp->f_lock);
        // ... 继续初始化 ...
        pthread_mutex_unlock(&fp->f_lock);  // 完成初始化后解锁
    }
    return (fp);  // 返回新分配的 foo 实例的指针
}

// foo_hold 函数用于增加对象的引用计数
// 参数:
//   fp: 指向 foo 结构体的指针
void foo_hold(struct foo *fp) /* add a reference to the object */
{
    // 加锁以确保引用计数的原子性操作
    pthread_mutex_lock(&fp->f_lock);

    // 增加引用计数
    fp->f_count++;

    // 解锁
    pthread_mutex_unlock(&fp->f_lock);
}
// foo_find 函数用于根据给定的 id 查找已存在的对象.
// 它首先锁定哈希表的互斥锁,然后遍历哈希表链表,寻找具有相同 id 的对象.
// 如果找到,则增加该对象的引用计数并解锁互斥锁,最后返回该对象的指针.
// 如果没有找到,则直接解锁互斥锁并返回 NULL.
struct foo *
foo_find(int id) /* find an existing object */
{
    struct foo *fp;  // 定义一个指向 foo 结构体的指针

    pthread_mutex_lock(&hashlock);                        // 锁定哈希表的互斥锁,以保证线程安全
    for (fp = fh[HASH(id)]; fp != NULL; fp = fp->f_next)  // 遍历哈希表对应的链表
    {
        if (fp->f_id == id)  // 如果找到具有相同 id 的对象
        {
            foo_hold(fp);  // 增加该对象的引用计数
            break;         // 跳出循环
        }
    }
    pthread_mutex_unlock(&hashlock);  // 解锁哈希表的互斥锁
    return (fp);                      // 返回找到的对象指针,如果没有找到则返回 NULL
}
void foo_rele(struct foo *fp) /* 释放对象的引用 */
{
    struct foo *tfp;  // 临时指针,用于遍历哈希链表
    int         idx;  // 哈希索引

    pthread_mutex_lock(&fp->f_lock);  // 锁定当前对象的互斥锁
    if (fp->f_count == 1)
    {                                       /* 最后一个引用 */
        pthread_mutex_unlock(&fp->f_lock);  // 先解锁当前对象的互斥锁
        pthread_mutex_lock(&hashlock);      // 锁定全局哈希表的互斥锁
        pthread_mutex_lock(&fp->f_lock);    // 再次锁定当前对象的互斥锁
        /* 需要重新检查条件 */
        if (fp->f_count != 1)
        {
            fp->f_count--;                      // 引用计数减一
            pthread_mutex_unlock(&fp->f_lock);  // 解锁当前对象的互斥锁
            pthread_mutex_unlock(&hashlock);    // 解锁全局哈希表的互斥锁
            return;                             // 如果不是最后一个引用,则直接返回
        }
        /* 从链表中移除 */
        idx = HASH(fp->f_id);  // 计算哈希索引
        tfp = fh[idx];         // 获取哈希链表的头节点
        if (tfp == fp)
        {
            fh[idx] = fp->f_next;  // 如果头节点就是要删除的节点,更新头节点
        }
        else
        {
            while (tfp->f_next != fp)  // 遍历链表找到要删除的节点
                tfp = tfp->f_next;
            tfp->f_next = fp->f_next;  // 将要删除的节点从链表中移除
        }
        pthread_mutex_unlock(&hashlock);     // 解锁全局哈希表的互斥锁
        pthread_mutex_unlock(&fp->f_lock);   // 解锁当前对象的互斥锁
        pthread_mutex_destroy(&fp->f_lock);  // 销毁当前对象的互斥锁
        free(fp);                            // 释放对象占用的内存
    }
    else
    {
        fp->f_count--;                      // 引用计数减一
        pthread_mutex_unlock(&fp->f_lock);  // 解锁当前对象的互斥锁
    }
}
