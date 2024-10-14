#include "slock.h"   // 引入自旋锁的头文件
#include <stdlib.h>  // 引入标准库中的内存分配函数
#include <stdio.h>   // 引入标准输入输出库
#include <unistd.h>  // 引入系统调用库,如sleep函数
#include <errno.h>   // 引入错误处理库,用于获取系统调用的错误码

// s_alloc 函数用于分配并初始化一个 slock 结构体实例.
// 它首先尝试分配内存,然后尝试创建一个唯一的命名信号量.
// 如果信号量已经存在,则继续尝试生成新的名称,直到成功创建或达到某种限制.
// 成功创建信号量后,会立即删除它,因为我们将使用 sem_init 在进程内初始化它.
struct slock *
s_alloc()
{
    struct slock *sp;   // 指向新分配的 slock 结构体的指针
    static int    cnt;  // 静态变量,用于生成唯一的信号量名称

    // 分配内存空间
    if ((sp = malloc(sizeof(struct slock))) == NULL)
        return (NULL);  // 如果分配失败,返回 NULL

    // 尝试创建唯一的命名信号量
    do
    {
        // 生成信号量名称
        snprintf(sp->name, sizeof(sp->name), "/%ld.%d", (long)getpid(), cnt++);
        // 尝试创建信号量
        sp->semp = sem_open(sp->name, O_CREAT | O_EXCL, S_IRWXU, 1);
    } while ((sp->semp == SEM_FAILED) && (errno == EEXIST));  // 如果信号量已存在,则重试

    // 检查信号量是否成功创建
    if (sp->semp == SEM_FAILED)
    {
        free(sp);       // 释放之前分配的内存
        return (NULL);  // 返回 NULL 表示失败
    }

    sem_unlink(sp->name);  // 删除命名信号量,因为我们将在进程内使用 sem_init 初始化它
    return (sp);           // 返回指向新分配并初始化的 slock 结构体的指针
}

// s_free 函数用于释放结构体 slock 的内存,并关闭与之关联的信号量.
// 参数 sp 是指向 slock 结构体的指针.
void s_free(struct slock *sp)
{
    // 关闭与 sp 关联的信号量.
    sem_close(sp->semp);

    // 释放 sp 指向的内存.
    free(sp);
}

/**
 * @brief 对结构体slock中的信号量进行加锁操作
 *
 * @param sp 指向slock结构体的指针
 * @return int 返回sem_wait函数的返回值,通常用于表示操作是否成功
 */
int s_lock(struct slock *sp)
{
    // 调用sem_wait函数对sp指向的结构体中的信号量进行加锁
    return (sem_wait(sp->semp));
}

/**
 * 尝试获取信号量锁.
 * @param sp 指向slock结构体的指针,该结构体包含一个信号量.
 * @return 如果成功获取锁则返回0,否则返回非0值表示失败.
 */
int s_trylock(struct slock *sp)
{
    // 调用sem_trywait函数尝试获取信号量锁
    return (sem_trywait(sp->semp));
}

// s_unlock 函数用于释放信号量,从而允许其他等待该信号量的线程继续执行.
// 参数 sp 是一个指向 slock 结构体的指针,该结构体包含了信号量的相关信息.
//
// 返回值:sem_post 函数的返回值,通常表示操作是否成功.
int s_unlock(struct slock *sp)
{
    // 调用 sem_post 函数释放信号量
    return (sem_post(sp->semp));
}
