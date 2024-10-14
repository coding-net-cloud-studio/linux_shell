#include "apue.h"        // 引入APUE库头文件
#include <poll.h>        // 引入poll函数头文件,用于I/O多路复用
#include <pthread.h>     // 引入pthread库头文件,用于多线程编程
#include <sys/msg.h>     // 引入消息队列系统调用头文件
#include <sys/socket.h>  // 引入套接字系统调用头文件

#define NQ     3      // 定义队列数量为3
#define MAXMSZ 512    // 定义消息最大大小为512字节
#define KEY    0x123  // 定义第一个消息队列的键值为0x123

// 定义线程信息结构体
// struct threadinfo 用于存储线程的ID和文件描述符
struct threadinfo
{
    int qid;  // 线程ID
    int fd;   // 文件描述符
};

// 定义消息结构体
// struct mymesg 用于存储消息类型和消息文本
struct mymesg
{
    long mtype;          // 消息类型
    char mtext[MAXMSZ];  // 消息文本,MAXMSZ是预定义的最大消息大小
};

void *
helper(void *arg)  // 定义helper函数,参数为void指针
{
    int                n;          // 定义整型变量n,用于存储msgrcv函数的返回值
    struct mymesg      m;          // 定义mymesg结构体变量m,用于接收消息队列的消息
    struct threadinfo *tip = arg;  // 定义threadinfo结构体指针tip,并将arg赋值给它,arg是helper函数的参数

    for (;;)  // 无限循环
    {
        memset(&m, 0, sizeof(m));                                    // 将m结构体清零
        if ((n = msgrcv(tip->qid, &m, MAXMSZ, 0, MSG_NOERROR)) < 0)  // 调用msgrcv函数从消息队列中接收消息,如果出错则返回负值
            err_sys("msgrcv error");                                 // 如果msgrcv函数出错,则调用err_sys函数输出错误信息
        if (write(tip->fd, m.mtext, n) < 0)                          // 将接收到的消息写入文件描述符tip->fd指向的文件中,如果写入失败则返回负值
            err_sys("write error");                                  // 如果write函数出错,则调用err_sys函数输出错误信息
    }
}

int main()
{
    int               i, n, err;    // 定义循环变量i,消息长度n,错误码err
    int               fd[2];        // 定义文件描述符数组fd,用于socketpair
    int               qid[NQ];      // 定义消息队列ID数组qid
    struct pollfd     pfd[NQ];      // 定义pollfd结构体数组pfd,用于轮询
    struct threadinfo ti[NQ];       // 定义线程信息结构体数组ti
    pthread_t         tid[NQ];      // 定义线程ID数组tid
    char              buf[MAXMSZ];  // 定义接收消息的缓冲区buf

    // 初始化消息队列,socketpair,pollfd结构体和线程
    for (i = 0; i < NQ; i++)
    {
        // 创建消息队列,如果失败则调用err_sys输出错误信息
        if ((qid[i] = msgget((KEY + i), IPC_CREAT | 0666)) < 0)
            err_sys("msgget error");

        // 打印消息队列ID
        printf("queue ID %d is %d\n", i, qid[i]);

        // 创建socketpair,如果失败则调用err_sys输出错误信息
        // 创建一对未命名的,相互连接的套接字,用于进程间通信
        // AF_UNIX 表示使用UNIX域套接字,SOCK_DGRAM 表示使用数据报套接字
        // fd 是一个整型数组,用于存储创建的套接字对
        // 如果函数返回值小于0,表示创建套接字失败,err_sys 函数用于输出错误信息并退出程序
        if (socketpair(AF_UNIX, SOCK_DGRAM, 0, fd) < 0)
            err_sys("socketpair error");

        // 设置pollfd结构体,指定文件描述符fd[0],并关注POLLIN事件(可读事件)
        pfd[i].fd     = fd[0];
        pfd[i].events = POLLIN;

        // 设置线程信息,将队列ID qid[i] 和文件描述符 fd[1] 赋值给线程信息结构体 ti[i]
        ti[i].qid = qid[i];
        ti[i].fd  = fd[1];

        // 创建线程,如果失败则调用err_exit输出错误信息
        // 创建一个新的线程
        // 如果创建线程失败,调用err_exit函数报告错误
        if ((err = pthread_create(&tid[i], NULL, helper, &ti[i])) != 0)
            err_exit(err, "pthread_create error");
    }

    // 无限循环,等待消息
    for (;;)
    {
        // 轮询,如果有事件发生则返回,否则阻塞
        if (poll(pfd, NQ, -1) < 0)
            err_sys("poll error");

        // 处理轮询到的事件
        for (i = 0; i < NQ; i++)  // 遍历所有文件描述符
        {
            // 如果有数据可读
            if (pfd[i].revents & POLLIN)
            {
                // 读取消息,如果失败则调用err_sys输出错误信息
                // 函数: read
                // 参数: pfd[i].fd - 文件描述符
                //       buf - 用于存储读取数据的缓冲区
                //       sizeof(buf) - 缓冲区大小
                // 返回值: 读取的字节数,如果出错返回-1
                if ((n = read(pfd[i].fd, buf, sizeof(buf))) < 0)
                    err_sys("read error");

                // 字符串结尾处理,确保buf是一个有效的C字符串
                buf[n] = 0;

                // 打印消息队列ID和消息内容
                printf("queue id %d, message %s\n", qid[i], buf);
            }
        }
    }

    // 正常退出程序
    exit(0);
}
