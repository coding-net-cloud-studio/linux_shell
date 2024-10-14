#include "apue.h"     // 引入APUE库的头文件
#include <sys/msg.h>  // 引入消息队列的系统头文件

// 定义消息结构的最大尺寸
#define MAXMSZ 512

// 定义一个消息结构体,包含消息类型和消息文本
struct mymesg
{
    // 消息类型,通常用于区分不同类型的消息
    long mtype;

    // 消息文本,存储具体的消息内容,最大长度为MAXMSZ
    char mtext[MAXMSZ];
};

int main(int argc, char *argv[])
{
    key_t         key;     // 定义消息队列的键
    long          qid;     // 定义消息队列的ID
    size_t        nbytes;  // 定义消息的字节数
    struct mymesg m;       // 定义消息结构体

    // 检查命令行参数的数量是否正确
    if (argc != 3)
    {
        fprintf(stderr, "usage: sendmsg KEY message\n");  // 输出错误信息
        exit(1);                                          // 退出程序
    }
    key = strtol(argv[1], NULL, 0);  // 将命令行参数转换为长整型
    // 获取消息队列ID,如果失败则输出错误信息并退出
    if ((qid = msgget(key, 0)) < 0)
        err_sys("can't open queue key %s", argv[1]);
    memset(&m, 0, sizeof(m));  // 清空消息结构体
    // 将命令行参数中的消息复制到消息结构体中
    strncpy(m.mtext, argv[2], MAXMSZ - 1);
    nbytes  = strlen(m.mtext);  // 获取消息的长度
    m.mtype = 1;                // 设置消息类型
    // 发送消息到消息队列,如果失败则输出错误信息
    if (msgsnd(qid, &m, nbytes, 0) < 0)
        err_sys("can't send message");
    exit(0);  // 发送成功,退出程序
}
