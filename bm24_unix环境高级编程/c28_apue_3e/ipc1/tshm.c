#include "apue.h"     // 引入APUE库头文件
#include <sys/shm.h>  // 引入系统共享内存头文件

// 定义数组大小为40000
#define ARRAY_SIZE  40000
// 定义动态分配的内存大小为100000
#define MALLOC_SIZE 100000
// 定义共享内存的大小为100000
#define SHM_SIZE    100000
// 定义共享内存的权限为用户读/写
#define SHM_MODE    0600

char array[ARRAY_SIZE]; /* 未初始化的数据 = bss */

int main(void)
{
    int   shmid;         // 共享内存标识符
    char *ptr, *shmptr;  // ptr用于存储malloc分配的内存地址,shmptr用于存储共享内存的地址

    // 打印数组在栈上的地址范围
    printf("array[] from %p to %p\n", (void *)&array[0], (void *)&array[ARRAY_SIZE]);
    // 打印栈周围的内存地址,用于观察栈的使用情况
    printf("stack around %p\n", (void *)&shmid);

    // 分配一块内存,并检查是否成功
    if ((ptr = malloc(MALLOC_SIZE)) == NULL)
        err_sys("malloc error");
    // 打印malloc分配的内存地址范围
    printf("malloced from %p to %p\n", (void *)ptr, (void *)ptr + MALLOC_SIZE);

    // 创建共享内存段,并检查是否成功
    if ((shmid = shmget(IPC_PRIVATE, SHM_SIZE, SHM_MODE)) < 0)
        err_sys("shmget error");
    // 将共享内存段附加到进程的地址空间,并检查是否成功
    if ((shmptr = shmat(shmid, 0, 0)) == (void *)-1)
        err_sys("shmat error");
    // 打印附加的共享内存地址范围
    printf("shared memory attached from %p to %p\n", (void *)shmptr, (void *)shmptr + SHM_SIZE);

    // 删除共享内存段,并检查是否成功
    if (shmctl(shmid, IPC_RMID, 0) < 0)
        err_sys("shmctl error");

    exit(0);
}
