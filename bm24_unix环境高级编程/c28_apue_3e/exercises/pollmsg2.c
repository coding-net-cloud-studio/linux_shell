#include "apue.h"        // 引入APUE库的头文件
#include <poll.h>        // 引入poll函数相关的头文件
#include <pthread.h>     // 引入线程相关的头文件
#include <sys/msg.h>     // 引入消息队列相关的头文件
#include <sys/socket.h>  // 引入套接字相关的头文件

#define NQ     3 /* number of queues */                 // 定义消息队列的数量
#define MAXMSZ 512 /* maximum message size */           // 定义消息的最大大小
#define KEY    0x123 /* key for first message queue */  // 定义第一个消息队列的键值

// 定义消息结构体,包含消息类型和消息文本
struct mymesg
{
    long mtype;              // 消息类型
    char mtext[MAXMSZ + 1];  // 消息文本,MAXMSZ为预定义的最大消息大小
};

// 定义线程信息结构体,包含线程相关的信息
struct threadinfo
{
    int             qid;    // 线程ID
    int             fd;     // 文件描述符
    int             len;    // 数据长度
    pthread_mutex_t mutex;  // 互斥锁,用于保护共享资源
    pthread_cond_t  ready;  // 条件变量,用于线程同步
    struct mymesg   m;      // 消息结构体实例
};

void *
helper(void *arg)
{
    // 定义整型变量n
    int n;
    // 将传入的参数arg转换为struct threadinfo指针tip
    struct threadinfo *tip = arg;

    // 无限循环
    for (;;)
    {
        // 清空tip指向的结构体成员m
        memset(&tip->m, 0, sizeof(struct mymsg));
        // 从消息队列tip->qid接收消息到tip->m,最多接收MAXMSZ字节,如果队列中没有消息则立即返回错误
        if ((n = msgrcv(tip->qid, &tip->m, MAXMSZ, 0, MSG_NOERROR)) < 0)
            // 如果msgrcv出错,调用err_sys函数报告错误
            err_sys("msgrcv error");
        // 将接收到的消息长度赋值给tip->len
        tip->len = n;
        // 加锁tip指向的互斥量
        pthread_mutex_lock(&tip->mutex);
        // 向tip->fd文件描述符写入一个字符'a'
        if (write(tip->fd, "a", sizeof(char)) < 0)
            // 如果write出错,调用err_sys函数报告错误
            err_sys("write error");
        // 等待tip指向的条件变量ready被唤醒
        pthread_cond_wait(&tip->ready, &tip->mutex);
        // 解锁tip指向的互斥量
        pthread_mutex_unlock(&tip->mutex);
    }
}
int main()
{
    char              c;          // 定义字符变量c
    int               i, n, err;  // 定义整型变量i, n, err
    int               fd[2];      // 定义文件描述符数组fd
    int               qid[NQ];    // 定义消息队列ID数组qid
    struct pollfd     pfd[NQ];    // 定义pollfd结构体数组pfd
    struct threadinfo ti[NQ];     // 定义线程信息结构体数组ti
    pthread_t         tid[NQ];    // 定义线程ID数组tid

    // 初始化消息队列和相关资源
    for (i = 0; i < NQ; i++)
    {
        // 创建消息队列,如果失败则调用err_sys函数报告错误
        if ((qid[i] = msgget((KEY + i), IPC_CREAT | 0666)) < 0)
            err_sys("msgget error");

        // 打印消息队列ID
        printf("queue ID %d is %d\n", i, qid[i]);

        // 创建UNIX域套接字对,如果失败则调用err_sys函数报告错误
        if (socketpair(AF_UNIX, SOCK_DGRAM, 0, fd) < 0)
            err_sys("socketpair error");
        pfd[i].fd     = fd[0];   // 设置pollfd结构体的文件描述符
        pfd[i].events = POLLIN;  // 设置pollfd结构体的事件为POLLIN
        ti[i].qid     = qid[i];  // 设置线程信息结构体的消息队列ID
        ti[i].fd      = fd[1];   // 设置线程信息结构体的文件描述符
        // 初始化条件变量,如果失败则调用err_sys函数报告错误
        if (pthread_cond_init(&ti[i].ready, NULL) != 0)
            err_sys("pthread_cond_init error");
        // 初始化互斥锁,如果失败则调用err_sys函数报告错误
        if (pthread_mutex_init(&ti[i].mutex, NULL) != 0)
            err_sys("pthread_mutex_init error");
        // 创建线程,如果失败则调用err_exit函数报告错误
        if ((err = pthread_create(&tid[i], NULL, helper, &ti[i])) != 0)
            err_exit(err, "pthread_create error");
    }

    // 主循环,等待事件发生
    for (;;)
    {
        // 使用poll函数等待事件,如果失败则调用err_sys函数报告错误
        if (poll(pfd, NQ, -1) < 0)
            err_sys("poll error");
        // 检查每个文件描述符的事件
        for (i = 0; i < NQ; i++)
        {
            if (pfd[i].revents & POLLIN)  // 如果事件为POLLIN
            {
                // 读取数据,如果失败则调用err_sys函数报告错误
                if ((n = read(pfd[i].fd, &c, sizeof(char))) < 0)
                    err_sys("read error");
                ti[i].m.mtext[ti[i].len] = 0;  // 设置消息文本结束符
                // 打印消息队列ID和消息内容
                printf("queue id %d, message %s\n", qid[i], ti[i].m.mtext);
                // 加锁互斥锁
                pthread_mutex_lock(&ti[i].mutex);
                // 发送条件变量信号
                pthread_cond_signal(&ti[i].ready);
                // 解锁互斥锁
                pthread_mutex_unlock(&ti[i].mutex);
            }
        }
    }

    exit(0);  // 程序正常退出
}
