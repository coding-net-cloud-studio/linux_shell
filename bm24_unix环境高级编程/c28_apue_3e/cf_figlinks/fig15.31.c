#include "apue.h"     // 引入APUE库头文件
#include <sys/shm.h>  // 引入系统共享内存头文件

#define ARRAY_SIZE  40000   // 定义数组大小为40000
#define MALLOC_SIZE 100000  // 定义动态内存分配大小为100000
#define SHM_SIZE    100000  // 定义共享内存大小为100000
#define SHM_MODE    0600    // 定义共享内存模式为用户读写权限

char array[ARRAY_SIZE];  // 定义一个未初始化的数据数组,属于bss段

int main(void)
{
    int   shmid;         // 定义共享内存标识符
    char *ptr, *shmptr;  // 定义指向malloc分配的内存和共享内存的指针

    // 打印数组在栈上的地址范围
    printf("array[] from %p to %p\n", (void *)&array[0], (void *)&array[ARRAY_SIZE]);
    // 打印栈周围的地址,用于比较内存布局
    printf("stack around %p\n", (void *)&shmid);

    // 分配一块动态内存,并检查是否成功
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

    // 删除共享内存段
    if (shmctl(shmid, IPC_RMID, 0) < 0)
        err_sys("shmctl error");

    exit(0);  // 退出程序
}
