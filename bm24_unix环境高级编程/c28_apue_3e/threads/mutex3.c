#include <stdlib.h>
#include <pthread.h>

// 定义哈希表的大小
#define NHASH    29

// 定义一个简单的哈希函数
#define HASH(id) (((unsigned long)id) % NHASH)

// 哈希表的数组,每个元素是一个指向foo结构体的指针
struct foo *fh[NHASH];

// 初始化一个互斥锁,用于保护哈希表的访问
pthread_mutex_t hashlock = PTHREAD_MUTEX_INITIALIZER;

// foo结构体定义
struct foo
{
    int             f_count;  // 引用计数,受hashlock保护
    pthread_mutex_t f_lock;   // 每个foo实例的互斥锁
    int             f_id;     // foo实例的唯一标识符
    struct foo     *f_next;   // 指向下一个foo实例的指针,受hashlock保护
                              /* ... more stuff here ... */
};
struct foo *
foo_alloc(int id) /* 分配对象 */
{
    struct foo *fp;   // 指向新分配的foo结构体的指针
    int         idx;  // 哈希表索引

    // 分配内存空间
    if ((fp = malloc(sizeof(struct foo))) != NULL)
    {
        fp->f_count = 1;   // 初始化引用计数为1
        fp->f_id    = id;  // 设置对象的ID

        // 初始化互斥锁,如果失败则释放内存并返回NULL
        // 初始化互斥锁,如果初始化失败,则释放fp指向的内存,并返回NULL
        // pthread_mutex_init: 初始化互斥锁
        // &fp->f_lock: 指向要初始化的互斥锁的指针
        // NULL: 使用默认属性初始化互斥锁
        if (pthread_mutex_init(&fp->f_lock, NULL) != 0)
        {
            free(fp);       // 初始化失败,释放fp指向的内存
            return (NULL);  // 返回NULL表示失败
        }

        idx = HASH(id);  // 计算ID的哈希值

        // 锁定全局哈希表锁
        pthread_mutex_lock(&hashlock);
        fp->f_next = fh[idx];             // 将新对象链接到哈希表的相应位置
        fh[idx]    = fp;                  // 更新哈希表的头部指针
        pthread_mutex_unlock(&hashlock);  // 解锁全局哈希表锁

        // 锁定对象的互斥锁以继续初始化
        pthread_mutex_lock(&fp->f_lock);
        // ...继续初始化...
        pthread_mutex_unlock(&fp->f_lock);  // 完成初始化后解锁对象的互斥锁
    }
    return (fp);  // 返回新分配的对象指针
}

// foo_hold 函数用于增加对象的引用计数
// 参数 fp 是指向 foo 结构体的指针
void foo_hold(struct foo *fp) /* add a reference to the object */
{
    // 加锁,保证引用计数的原子性操作
    pthread_mutex_lock(&hashlock);

    // 增加对象的引用计数
    fp->f_count++;

    // 解锁,释放互斥锁
    pthread_mutex_unlock(&hashlock);
}
// foo_find 函数用于根据给定的 id 查找已存在的对象.
// 它首先锁定互斥锁以确保线程安全,然后遍历哈希表以查找匹配的对象.
// 如果找到匹配的对象,它会增加该对象的引用计数并返回该对象的指针.
// 如果没有找到匹配的对象,则返回 NULL.
struct foo *
foo_find(int id) /* find an existing object */
{
    struct foo *fp;  // 定义一个指向 foo 结构体的指针

    pthread_mutex_lock(&hashlock);  // 锁定互斥锁,以保证在多线程环境下的数据一致性
    // 遍历哈希表中与给定 id 相关的链表
    for (fp = fh[HASH(id)]; fp != NULL; fp = fp->f_next)
    {
        // 如果找到了具有相同 id 的对象
        if (fp->f_id == id)
        {
            fp->f_count++;  // 增加该对象的引用计数
            break;          // 跳出循环
        }
    }
    pthread_mutex_unlock(&hashlock);  // 解锁互斥锁
    return (fp);                      // 返回找到的对象指针,如果没有找到则返回 NULL
}
void foo_rele(struct foo *fp) /* 释放对象的引用 */
{
    struct foo *tfp;  // 临时指针,用于遍历哈希链表
    int         idx;  // 哈希索引

    pthread_mutex_lock(&hashlock);  // 加锁保护哈希表
    if (--fp->f_count == 0)         // 如果引用计数减到0
    {                               /* 最后一个引用,从链表中移除 */
        idx = HASH(fp->f_id);       // 计算哈希值
        tfp = fh[idx];              // 获取哈希桶的头节点
        if (tfp == fp)              // 如果头节点就是要删除的节点
        {
            fh[idx] = fp->f_next;  // 直接将头节点指向下一个节点
        }
        else  // 如果不是头节点
        {
            while (tfp->f_next != fp)  // 遍历链表找到要删除的节点的前一个节点
                tfp = tfp->f_next;
            tfp->f_next = fp->f_next;  // 将前一个节点的next指向要删除节点的下一个节点
        }
        pthread_mutex_unlock(&hashlock);     // 解锁
        pthread_mutex_destroy(&fp->f_lock);  // 销毁对象的锁
        free(fp);                            // 释放对象内存
    }
    else  // 如果引用计数未减到0
    {
        pthread_mutex_unlock(&hashlock);  // 解锁
    }
}
