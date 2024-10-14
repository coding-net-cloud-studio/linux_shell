#include <stdlib.h>   // 引入标准库,提供内存分配等功能
#include <pthread.h>  // 引入POSIX线程库,提供多线程编程支持

// 定义哈希表的大小
#define NHASH    29

// 定义一个宏,用于计算给定id的哈希值
#define HASH(id) (((unsigned long)id) % NHASH)

// 定义一个结构体指针数组,用于存储哈希表的桶
struct foo *fh[NHASH];

// 初始化一个互斥锁,用于在多线程环境下保护哈希表的访问
pthread_mutex_t hashlock = PTHREAD_MUTEX_INITIALIZER;

// 定义一个名为foo的结构体
struct foo
{
    int             f_count;  // 计数器,用于记录结构体实例的数量
    pthread_mutex_t f_lock;   // 互斥锁,用于保护结构体的数据安全
    int             f_id;     // 标识符,每个结构体实例的唯一ID
    struct foo     *f_next;   // 指向下一个foo结构体实例的指针,受hashlock保护
                              // ... 这里可能有更多的成员 ...
};

// foo_alloc 函数用于分配一个新的 foo 结构体实例,并进行初始化.
// 参数:
//   id - 要分配的 foo 实例的唯一标识符.
// 返回值:
//   成功时返回指向新分配 foo 实例的指针,失败时返回 NULL.
struct foo *
foo_alloc(int id) /* allocate the object */
{
    struct foo *fp;   // 指向新分配的 foo 结构体的指针
    int         idx;  // 用于存储哈希表索引的变量

    // 分配内存空间给新的 foo 结构体
    if ((fp = malloc(sizeof(struct foo))) != NULL)
    {
        fp->f_count = 1;   // 初始化引用计数为 1
        fp->f_id    = id;  // 设置 foo 实例的 ID

        // 初始化 foo 实例的互斥锁
        if (pthread_mutex_init(&fp->f_lock, NULL) != 0)
        {
            free(fp);       // 如果互斥锁初始化失败,释放已分配的内存
            return (NULL);  // 返回 NULL 表示失败
        }

        idx = HASH(id);  // 计算 ID 的哈希值,得到哈希表索引

        // 锁定全局哈希表锁,以便安全地将新实例插入哈希表
        pthread_mutex_lock(&hashlock);
        fp->f_next = fh[idx];             // 将新实例链接到哈希表的相应位置
        fh[idx]    = fp;                  // 更新哈希表的头部指针
        pthread_mutex_unlock(&hashlock);  // 解锁全局哈希表锁

        // 锁定新实例的互斥锁,继续初始化
        pthread_mutex_lock(&fp->f_lock);
        // ... 继续初始化 ...
        pthread_mutex_unlock(&fp->f_lock);  // 完成初始化后解锁新实例的互斥锁
    }
    return (fp);  // 返回新分配的 foo 实例指针
}

// foo_hold 函数用于增加对象的引用计数
// 参数:
//   fp: 指向 foo 结构体的指针,表示要增加引用计数的对象
void foo_hold(struct foo *fp) /* add a reference to the object */
{
    // 加锁,保证引用计数的原子性操作
    pthread_mutex_lock(&fp->f_lock);

    // 增加引用计数
    fp->f_count++;

    // 解锁
    pthread_mutex_unlock(&fp->f_lock);
}

// foo_find 函数用于查找具有特定 id 的对象
// 参数:
//   id: 要查找的对象的 id
// 返回值:
//   如果找到对象,则返回指向该对象的指针;否则返回 NULL
struct foo *
foo_find(int id) /* find an existing object */
{
    struct foo *fp;  // 用于遍历的指针

    pthread_mutex_lock(&hashlock);  // 加锁,保证线程安全
    // 遍历哈希表中与 id 对应的链表
    for (fp = fh[HASH(id)]; fp != NULL; fp = fp->f_next)
    {
        if (fp->f_id == id)  // 如果找到具有相同 id 的对象
        {
            foo_hold(fp);  // 增加对象的引用计数
            break;         // 跳出循环
        }
    }
    pthread_mutex_unlock(&hashlock);  // 解锁
    return (fp);                      // 返回找到的对象指针或 NULL
}

void foo_rele(struct foo *fp) /* 释放对象的引用 */
{
    struct foo *tfp;  // 临时指针,用于遍历哈希表
    int         idx;  // 哈希索引

    pthread_mutex_lock(&fp->f_lock);  // 加锁,保证线程安全
    if (fp->f_count == 1)
    {                                       /* 最后一个引用 */
        pthread_mutex_unlock(&fp->f_lock);  // 先解锁
        pthread_mutex_lock(&hashlock);      // 加全局哈希表锁
        pthread_mutex_lock(&fp->f_lock);    // 再次加锁,确保在检查和操作期间没有其他线程修改
        /* 需要重新检查条件 */
        if (fp->f_count != 1)  // 如果在此期间引用计数发生变化,则减少计数并返回
        {
            // 减少文件指针的引用计数
            fp->f_count--;

            // 解锁文件指针的互斥锁,允许其他线程访问文件指针
            pthread_mutex_unlock(&fp->f_lock);

            // 解锁哈希表的互斥锁,允许其他线程访问哈希表
            pthread_mutex_unlock(&hashlock);

            // 完成操作后返回
            return;
        }
        /* 从列表中移除 */
        idx = HASH(fp->f_id);  // 计算哈希索引
        tfp = fh[idx];         // 获取哈希表对应位置的指针
        if (tfp == fp)         // 如果是链表头
        {
            fh[idx] = fp->f_next;  // 直接移除
        }
        else  // 否则遍历链表移除
        {
            // 以下代码段位于文件fig11.11.c中

            // 循环遍历文件指针列表,直到找到指向当前文件指针fp的前一个文件指针tfp
            while (tfp->f_next != fp)
                tfp = tfp->f_next;

            // 将fp的前一个文件指针tfp的f_next指针指向fp的下一个文件指针,从而从链表中移除fp
            tfp->f_next = fp->f_next;
        }
        pthread_mutex_unlock(&hashlock);     // 解锁全局哈希表锁
        pthread_mutex_unlock(&fp->f_lock);   // 解锁对象锁
        pthread_mutex_destroy(&fp->f_lock);  // 销毁对象锁
        free(fp);                            // 释放对象内存
    }
    else  // 如果不是最后一个引用,只需减少计数
    {
        // 减少文件指针fp的引用计数
        // 这意味着文件的使用次数减少了一次
        fp->f_count--;

        // 解锁文件指针fp的互斥锁
        // 允许其他线程访问和修改文件指针fp
        pthread_mutex_unlock(&fp->f_lock);
    }
}
