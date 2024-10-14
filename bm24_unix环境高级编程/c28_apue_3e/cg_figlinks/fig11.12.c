#include <stdlib.h>   // 引入标准库,提供内存分配等功能
#include <pthread.h>  // 引入POSIX线程库,用于多线程编程

// 定义哈希表的大小
#define NHASH    29

// 定义一个宏,用于计算id的哈希值
#define HASH(id) (((unsigned long)id) % NHASH)

// 声明一个结构体指针数组,用于存储哈希表的桶
struct foo *fh[NHASH];

// 初始化一个互斥锁,用于保护哈希表的并发访问
pthread_mutex_t hashlock = PTHREAD_MUTEX_INITIALIZER;

struct foo
{
    int             f_count;  // 由hashlock保护,表示foo实例的引用计数
    pthread_mutex_t f_lock;   // 互斥锁,用于保护f_count和f_next的并发访问
    int             f_id;     // foo实例的唯一标识符
    struct foo     *f_next;   // 指向下一个foo实例的指针,由hashlock保护
                              // ... 更多的成员变量 ...
};

// foo_alloc - 分配一个新的foo结构体实例
// @param id: 要分配的foo结构体的ID
// @return: 分配成功返回指向foo结构体的指针,失败返回NULL
struct foo *
foo_alloc(int id) /* allocate the object */
{
    struct foo *fp;   // 指向新分配的foo结构体的指针
    int         idx;  // 哈希表索引

    // 分配内存空间
    if ((fp = malloc(sizeof(struct foo))) != NULL)
    {
        fp->f_count = 1;   // 初始化引用计数为1
        fp->f_id    = id;  // 设置foo结构体的ID

        // 初始化互斥锁
        if (pthread_mutex_init(&fp->f_lock, NULL) != 0)
        {
            free(fp);       // 初始化失败,释放内存
            return (NULL);  // 返回NULL
        }

        idx = HASH(id);  // 计算哈希值

        // 加锁全局哈希表
        pthread_mutex_lock(&hashlock);
        fp->f_next = fh[idx];             // 将新分配的foo结构体链接到哈希表
        fh[idx]    = fp;                  // 更新哈希表的头部指针
        pthread_mutex_unlock(&hashlock);  // 解锁全局哈希表

        // 加锁新分配的foo结构体的互斥锁
        pthread_mutex_lock(&fp->f_lock);
        // ... 继续初始化 ...
        pthread_mutex_unlock(&fp->f_lock);  // 解锁新分配的foo结构体的互斥锁
    }
    return (fp);  // 返回指向新分配的foo结构体的指针
}

// foo_hold 函数用于增加对象的引用计数
// 参数:
//   fp: 指向 foo 结构体的指针,表示要增加引用计数的对象
void foo_hold(struct foo *fp)
{
    // 加锁,保证引用计数的原子性操作
    pthread_mutex_lock(&hashlock);
    // 增加对象的引用计数
    fp->f_count++;
    // 解锁
    pthread_mutex_unlock(&hashlock);
}

// foo_find 函数用于根据给定的 id 查找已存在的对象.
// 它首先锁定哈希表的互斥锁,然后遍历哈希表链表,
// 如果找到匹配的对象,则增加其引用计数并返回该对象的指针.
// 如果没有找到,则返回 NULL.
struct foo *
foo_find(int id) /* find an existing object */
{
    struct foo *fp;  // 定义一个指向 foo 结构体的指针

    pthread_mutex_lock(&hashlock);                        // 锁定哈希表的互斥锁,以保证线程安全
    for (fp = fh[HASH(id)]; fp != NULL; fp = fp->f_next)  // 遍历哈希表对应桶的链表
    {
        if (fp->f_id == id)  // 如果找到 id 匹配的对象
        {
            fp->f_count++;  // 增加该对象的引用计数
            break;          // 跳出循环
        }
    }
    pthread_mutex_unlock(&hashlock);  // 解锁哈希表的互斥锁
    return (fp);                      // 返回找到的对象指针,如果没有找到则返回 NULL
}

void foo_rele(struct foo *fp) /* release a reference to the object */
{
    struct foo *tfp;  // 定义一个指向foo结构体的指针tfp
    int         idx;  // 定义一个整型变量idx用于存储哈希值

    pthread_mutex_lock(&hashlock);  // 加锁,保证线程安全
    if (--fp->f_count == 0)         // 如果引用计数减到0
    {                               /* last reference, remove from list */
        idx = HASH(fp->f_id);       // 计算fp的哈希值
        tfp = fh[idx];              // 获取哈希表中对应的链表头
        if (tfp == fp)              // 如果链表头就是fp
        {
            fh[idx] = fp->f_next;  // 直接将链表头指向下一个节点
        }
        else  // 否则
        {
            while (tfp->f_next != fp)  // 遍历链表找到fp的前一个节点
                tfp = tfp->f_next;
            tfp->f_next = fp->f_next;  // 将fp的前一个节点的next指向fp的下一个节点
        }
        pthread_mutex_unlock(&hashlock);     // 解锁
        pthread_mutex_destroy(&fp->f_lock);  // 销毁fp的锁
        free(fp);                            // 释放fp占用的内存
    }
    else  // 如果引用计数没有减到0
    {
        pthread_mutex_unlock(&hashlock);  // 直接解锁
    }
}
