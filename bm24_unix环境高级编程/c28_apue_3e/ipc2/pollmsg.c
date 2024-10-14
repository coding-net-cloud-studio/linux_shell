#include "apue.h"        // 引入APUE库的头文件
#include <poll.h>        // 引入poll函数相关的头文件
#include <pthread.h>     // 引入POSIX线程库的头文件
#include <sys/msg.h>     // 引入消息队列相关的头文件
#include <sys/socket.h>  // 引入套接字编程相关的头文件

// 定义队列数量
#define NQ     3
// 定义消息最大大小
#define MAXMSZ 512
// 定义第一个消息队列的键值
#define KEY    0x123

// 线程信息结构体
// qid: 队列ID
// fd: 文件描述符
struct threadinfo
{
    int qid;
    int fd;
};

// 自定义消息结构体
// mtype: 消息类型
// mtext: 消息文本,最大长度为MAXMSZ
struct mymesg
{
    long mtype;
    char mtext[MAXMSZ];
};

void *
helper(void *arg)
{

    // 定义整型变量n,用于存储消息数量或其他整型数据
    int n;

    // 定义结构体变量m,类型为mymesg,用于存储消息数据
    struct mymesg m;

    // 定义指向threadinfo结构体的指针变量tip,初始化为arg,用于存储线程信息
    struct threadinfo *tip = arg;

    // 无限循环,持续监听消息队列
    for (;;)
    {
        // 清空消息结构体
        memset(&m, 0, sizeof(m));

        // 从消息队列接收消息,如果接收失败则输出错误信息并退出
        // msgrcv函数用于从消息队列中接收消息
        // tip->qid是消息队列的标识符
        // &m是接收消息的缓冲区
        // MAXMSZ是消息的最大长度
        // 0表示默认的消息类型
        // MSG_NOERROR表示如果消息队列已满,不等待而立即返回错误
        if ((n = msgrcv(tip->qid, &m, MAXMSZ, 0, MSG_NOERROR)) < 0)
            err_sys("msgrcv error");

        // 将接收到的消息写入文件描述符tip->fd指向的文件或设备
        // 如果写入失败则输出错误信息并退出
        // write函数用于将数据写入文件描述符
        if (write(tip->fd, m.mtext, n) < 0)
            err_sys("write error");
    }
}

int main()
{

    // 定义整型变量i, n, err用于循环控制和错误处理
    int i, n, err;

    // 定义整型数组fd用于存储文件描述符
    int fd[2];

    // 定义整型数组qid用于存储消息队列ID
    int qid[NQ];

    // 定义pollfd结构体数组pfd,用于轮询文件描述符的状态
    struct pollfd pfd[NQ];

    // 定义threadinfo结构体数组ti,用于存储线程信息
    struct threadinfo ti[NQ];

    // 定义pthread_t类型数组tid,用于存储线程ID
    pthread_t tid[NQ];

    // 定义字符数组buf,用于存储消息缓冲区
    char buf[MAXMSZ];

    // 循环创建消息队列和对应的线程
    for (i = 0; i < NQ; i++)
    {
        // 获取或创建消息队列,KEY+i为队列键值,IPC_CREAT表示若不存在则创建,0666为权限
        if ((qid[i] = msgget((KEY + i), IPC_CREAT | 0666)) < 0)
            err_sys("msgget error");  // 如果出错,调用err_sys打印错误信息

        printf("queue ID %d is %d\n", i, qid[i]);  // 打印消息队列ID

        // 创建一对匿名的UDP套接字,用于线程间通信
        if (socketpair(AF_UNIX, SOCK_DGRAM, 0, fd) < 0)
            err_sys("socketpair error");  // 如果出错,调用err_sys打印错误信息
        pfd[i].fd     = fd[0];            // 设置pollfd结构体中的文件描述符为套接字的读端
        pfd[i].events = POLLIN;           // 设置感兴趣的事件为可读事件
        ti[i].qid     = qid[i];           // 设置线程信息结构体中的消息队列ID
        ti[i].fd      = fd[1];            // 设置线程信息结构体中的文件描述符为套接字的写端
        // 创建线程,执行helper函数,传入线程信息结构体的地址
        if ((err = pthread_create(&tid[i], NULL, helper, &ti[i])) != 0)
            err_exit(err, "pthread_create error");  // 如果出错,调用err_exit打印错误信息并退出
    }

    // 无限循环,等待事件发生
    for (;;)
    {
        // 使用poll函数等待文件描述符数组pfd中的事件,NQ是数组大小,-1表示无限等待
        // 如果poll返回值小于0,表示发生错误,调用err_sys打印错误信息并退出
        if (poll(pfd, NQ, -1) < 0)
            err_sys("poll error");

        // 遍历文件描述符数组
        for (i = 0; i < NQ; i++)
        {
            // 检查当前文件描述符是否有POLLIN事件,即是否有数据可读
            if (pfd[i].revents & POLLIN)
            {
                // 读取文件描述符pfd[i].fd中的数据到缓冲区buf,最多读取sizeof(buf)字节
                // 如果读取的字节数n小于0,表示发生错误,调用err_sys打印错误信息并退出
                if ((n = read(pfd[i].fd, buf, sizeof(buf))) < 0)
                    err_sys("read error");

                // 读取完成后,在缓冲区末尾添加字符串结束符'\0'
                buf[n] = 0;

                // 打印队列ID和读取到的消息
                printf("queue id %d, message %s\n", qid[i], buf);
            }
        }
    }

    // 程序正常退出
    exit(0);
}
