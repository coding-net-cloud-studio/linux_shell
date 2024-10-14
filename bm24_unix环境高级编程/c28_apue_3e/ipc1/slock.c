#include "slock.h"   // 包含自定义的锁头文件
#include <stdlib.h>  // 包含标准库函数,如malloc和free
#include <stdio.h>   // 包含标准输入输出函数,如printf和scanf
#include <unistd.h>  // 包含Unix系统服务的函数,如sleep
#include <errno.h>   // 包含错误码定义,用于错误处理

// 分配并初始化一个信号量锁
struct slock *
s_alloc()
{
    struct slock *sp;   // 定义信号量锁结构体指针
    static int    cnt;  // 定义静态计数器,用于生成唯一的信号量名称

    // 分配内存空间
    if ((sp = malloc(sizeof(struct slock))) == NULL)
        return (NULL);  // 内存分配失败,返回NULL

    // 尝试创建唯一的信号量
    do
    {
        // 生成唯一的信号量名称
        snprintf(sp->name, sizeof(sp->name), "/%ld.%d", (long)getpid(), cnt++);
        // 尝试创建信号量,O_CREAT | O_EXCL确保信号量不存在时才创建,S_IRWXU设置权限
        sp->semp = sem_open(sp->name, O_CREAT | O_EXCL, S_IRWXU, 1);
    } while ((sp->semp == SEM_FAILED) && (errno == EEXIST));  // 如果信号量已存在,继续尝试

    // 如果信号量创建失败
    if (sp->semp == SEM_FAILED)
    {
        free(sp);       // 释放已分配的内存
        return (NULL);  // 返回NULL
    }
    sem_unlink(sp->name);  // 删除信号量,但不关闭它,因为sem_close会在s_free中调用
    return (sp);           // 返回初始化好的信号量锁
}

// 释放信号量锁
void s_free(struct slock *sp)
{
    sem_close(sp->semp);  // 关闭信号量
    free(sp);             // 释放内存
}

// 获取信号量锁
int s_lock(struct slock *sp)
{
    return (sem_wait(sp->semp));  // 等待并获取信号量
}

// 尝试获取信号量锁,不阻塞
int s_trylock(struct slock *sp)
{
    return (sem_trywait(sp->semp));  // 尝试获取信号量,立即返回结果
}

// 释放信号量锁
int s_unlock(struct slock *sp)
{
    return (sem_post(sp->semp));  // 释放信号量
}
