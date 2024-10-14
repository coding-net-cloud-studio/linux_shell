#include "apue.h"

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
        // 从字符串line中读取两个整数到int1和int2
        // sscanf函数尝试将格式字符串"%d%d"匹配到line,并将匹配到的数据存储到int1和int2中
        // 如果成功读取了两个整数,函数返回2
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
