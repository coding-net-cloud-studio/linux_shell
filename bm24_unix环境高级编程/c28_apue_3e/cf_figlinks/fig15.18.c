#include "apue.h"

// 信号处理函数,用于处理管道破裂信号
static void sig_pipe(int); /* our signal handler */

// 主函数
int main(void)
{
    int  n, int1, int2;  // n用于存储读取的字节数,int1和int2用于存储解析的整数
    char line[MAXLINE];  // 用于存储从标准输入读取的行

    // 循环读取标准输入的每一行
    while ((n = read(STDIN_FILENO, line, MAXLINE)) > 0)
    {
        line[n] = 0;  // 确保读取的字符串以null结尾
        // 尝试从读取的行中解析两个整数
        if (sscanf(line, "%d%d", &int1, &int2) == 2)
        {
            // 如果成功解析两个整数,计算它们的和并格式化为字符串
            sprintf(line, "%d\n", int1 + int2);
            n = strlen(line);  // 获取格式化后字符串的长度
            // 将结果写入标准输出
            if (write(STDOUT_FILENO, line, n) != n)
                err_sys("write error");  // 如果写入失败,报告错误
        }
        else
        {
            // 如果未能解析两个整数,输出错误信息
            if (write(STDOUT_FILENO, "invalid args\n", 13) != 13)
                err_sys("write error");  // 如果写入失败,报告错误
        }
    }
    exit(0);  // 正常退出程序
}

// 定义一个信号处理函数,用于处理 SIGPIPE 信号
static void
sig_pipe(int signo)
{
    // 当捕获到 SIGPIPE 信号时,打印消息
    printf("SIGPIPE caught\n");
    // 退出程序,返回状态码 1
    exit(1);
}
