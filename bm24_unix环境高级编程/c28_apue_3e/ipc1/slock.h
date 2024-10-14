#include <semaphore.h>  // 包含信号量相关的头文件
#include <fcntl.h>      // 包含文件控制相关的头文件
#include <limits.h>     // 包含系统限制相关的头文件
#include <sys/stat.h>   // 包含文件状态相关的头文件

// 定义一个结构体slock,用于表示信号量锁
struct slock
{
    sem_t *semp;                   // 指向信号量的指针
    char   name[_POSIX_NAME_MAX];  // 信号量的名称
};

// 分配一个新的slock结构体实例
struct slock *s_alloc();

// 释放slock结构体实例占用的资源
void s_free(struct slock *);

// 尝试获取信号量锁,如果锁被占用则阻塞
int s_lock(struct slock *);

// 尝试获取信号量锁,如果锁被占用则立即返回失败
int s_trylock(struct slock *);

// 释放信号量锁
int s_unlock(struct slock *);
