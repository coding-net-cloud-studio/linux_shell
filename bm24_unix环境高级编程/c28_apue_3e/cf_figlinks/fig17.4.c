#include "apue.h"     // 引入apue库头文件
#include <sys/msg.h>  // 引入系统消息队列头文件

#define MAXMSZ 512  // 定义消息队列中消息的最大尺寸为512字节

// 定义一个消息结构体,用于在进程间传递消息
struct mymesg
{
    long mtype;          // 消息类型,用于区分不同的消息
    char mtext[MAXMSZ];  // 消息文本,存储具体的消息内容
};

/**
 * 主函数,用于发送消息到消息队列.
 * @param argc 命令行参数的数量
 * @param argv 命令行参数的数组
 * @return 返回0表示成功,非0表示失败
 */
int main(int argc, char *argv[])
{
    key_t         key;     // 消息队列的键值
    long          qid;     // 消息队列的标识符
    size_t        nbytes;  // 消息的字节数
    struct mymesg m;       // 自定义的消息结构体

    // 检查命令行参数的数量是否正确
    if (argc != 3)
    {
        fprintf(stderr, "usage: sendmsg KEY message\n");
        exit(1);
    }
    // 将命令行参数转换为键值
    key = strtol(argv[1], NULL, 0);
    // 获取消息队列的标识符
    if ((qid = msgget(key, 0)) < 0)
        err_sys("can't open queue key %s", argv[1]);
    // 清空消息结构体
    memset(&m, 0, sizeof(m));
    // 将命令行参数中的消息内容复制到消息结构体中
    strncpy(m.mtext, argv[2], MAXMSZ - 1);
    // 计算消息的字节数
    nbytes = strlen(m.mtext);
    // 设置消息类型
    m.mtype = 1;
    // 发送消息到消息队列

    // 发送消息到消息队列的函数调用.
    // 如果发送失败,将调用err_sys函数报告错误.
    // 参数说明:
    // qid: 消息队列的标识符.
    // &m: 要发送的消息的地址.
    // nbytes: 消息的字节数.
    // 0: 消息发送标志,0表示阻塞直到消息被发送.
    if (msgsnd(qid, &m, nbytes, 0) < 0)
        err_sys("can't send message");  // 如果msgsnd返回值小于0,表示发送失败,调用err_sys报告错误.

    // 成功发送消息后退出程序
    exit(0);
}
