#include <stdlib.h>
#include <pthread.h>

// 定义结构体foo,用于管理资源
struct foo
{
    int             f_count;       // 引用计数器,用于跟踪有多少线程正在访问该资源
    pthread_mutex_t f_lock;        // 互斥锁,用于保护对资源的访问
    int             f_id;          // 资源的唯一标识符
    /* ... more stuff here ... */  // 可能还有其他与资源相关的成员
};

// foo_alloc 函数用于分配一个新的 foo 结构体实例,并初始化其成员变量.
// 参数:
//   id - 要分配的 foo 实例的唯一标识符.
// 返回值:
//   如果分配成功并初始化,返回指向新分配的 foo 实例的指针;否则返回 NULL.
struct foo *
foo_alloc(int id) /* allocate the object */
{
    struct foo *fp;  // 指向新分配的 foo 结构体的指针

    // 尝试分配内存空间
    if ((fp = malloc(sizeof(struct foo))) != NULL)
    {
        fp->f_count = 1;   // 初始化引用计数为 1
        fp->f_id    = id;  // 设置 foo 实例的 ID

        // 初始化互斥锁,用于保护 foo 实例的并发访问
        if (pthread_mutex_init(&fp->f_lock, NULL) != 0)
        {
            free(fp);       // 如果互斥锁初始化失败,释放已分配的内存
            return (NULL);  // 并返回 NULL 表示失败
        }
        /* ... continue initialization ... */
        // 继续初始化其他成员变量...
    }
    return (fp);  // 返回新分配并初始化的 foo 实例指针
}

// foo_hold 函数用于增加对象的引用计数
// 参数:
//   fp: 指向 foo 结构体的指针,该结构体包含一个互斥锁和一个引用计数器
void foo_hold(struct foo *fp) /* add a reference to the object */
{
    // 加锁以确保引用计数的原子性操作
    pthread_mutex_lock(&fp->f_lock);

    // 增加引用计数
    fp->f_count++;

    // 解锁互斥锁
    pthread_mutex_unlock(&fp->f_lock);
}
void foo_rele(struct foo *fp) /* release a reference to the object */
{
    // 加锁,保证线程安全
    pthread_mutex_lock(&fp->f_lock);

    // 引用计数减一
    if (--fp->f_count == 0)
    { /* last reference */
        // 如果是最后一个引用,释放锁
        pthread_mutex_unlock(&fp->f_lock);
        // 销毁互斥锁
        pthread_mutex_destroy(&fp->f_lock);
        // 释放对象占用的内存
        free(fp);
    }
    else
    {
        // 如果不是最后一个引用,直接释放锁
        pthread_mutex_unlock(&fp->f_lock);
    }
}
