#include <stdlib.h>   // 引入标准库,提供内存分配等功能
#include <pthread.h>  // 引入POSIX线程库,提供多线程编程支持

// 定义了一个名为foo的结构体
struct foo
{
    // f_count: 用于计数或跟踪结构体实例的数量
    int f_count;

    // f_lock: 一个互斥锁,用于保护结构体的数据成员,防止并发访问时的竞态条件
    pthread_mutex_t f_lock;

    // f_id: 结构体的唯一标识符
    int f_id;

    /* ... more stuff here ... */
    // 注释中提到还有更多的成员变量,但在此省略
};

// foo_alloc 函数用于分配一个新的 foo 结构体实例.
// 参数:
//   id - 要分配的结构体的 ID.
// 返回值:
//   成功时返回指向新分配的 foo 结构体的指针,失败时返回 NULL.
struct foo *
foo_alloc(int id) /* allocate the object */
{
    struct foo *fp;  // 指向新分配的 foo 结构体的指针

    // 尝试为 foo 结构体分配内存
    if ((fp = malloc(sizeof(struct foo))) != NULL)
    {
        fp->f_count = 1;   // 初始化引用计数为 1
        fp->f_id    = id;  // 设置结构体的 ID

        // 初始化互斥锁,如果失败则释放内存并返回 NULL
        if (pthread_mutex_init(&fp->f_lock, NULL) != 0)
        {
            free(fp);
            return (NULL);
        }
        /* ... continue initialization ... */  // 继续初始化其他成员
    }
    return (fp);  // 返回新分配的结构体指针或 NULL
}

// foo_hold 函数用于增加对象的引用计数
// 参数:
//   fp: 指向 foo 结构体的指针
void foo_hold(struct foo *fp) /* add a reference to the object */
{
    // 加锁以确保引用计数的线程安全
    pthread_mutex_lock(&fp->f_lock);
    // 增加引用计数
    fp->f_count++;
    // 解锁
    pthread_mutex_unlock(&fp->f_lock);
}

// foo_rele 函数用于释放对对象的引用
// 参数: fp - 指向 foo 结构体的指针
void foo_rele(struct foo *fp) /* release a reference to the object */
{
    // 加锁,保证线程安全
    pthread_mutex_lock(&fp->f_lock);

    // 引用计数减一
    if (--fp->f_count == 0)
    { /* last reference */
        // 如果是最后一个引用,解锁并销毁互斥锁
        // 解锁文件指针fp的互斥锁
        pthread_mutex_unlock(&fp->f_lock);

        // 销毁文件指针fp的互斥锁,释放相关资源
        // 注意:在调用此函数之前,必须确保互斥锁未被锁定,否则可能导致未定义行为
        pthread_mutex_destroy(&fp->f_lock);

        // 释放对象占用的内存
        free(fp);
    }
    else
    {
        // 如果不是最后一个引用,仅解锁
        // 解锁文件指针fp的互斥锁,允许其他线程访问该文件指针.
        // 这通常在文件操作完成后调用,以确保线程安全.
        pthread_mutex_unlock(&fp->f_lock);
    }
}
