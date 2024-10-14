#include "apue.h"   // 引入apue库,提供系统调用的封装和错误处理函数

// 主函数
int main(void)
{
    int  n, int1, int2;  // 定义变量n, int1, int2
    char line[MAXLINE];  // 定义字符数组line,用于存储从标准输入读取的数据

    // 循环读取标准输入的数据
    while ((n = read(STDIN_FILENO, line, MAXLINE)) > 0)
    {
        line[n] = 0;  // 字符数组末尾添加空字符,使其成为一个C风格字符串
        // 尝试从读取的字符串中解析两个整数
        if (sscanf(line, "%d%d", &int1, &int2) == 2)
        {
            // 如果成功解析两个整数,计算它们的和,并将结果转换为字符串
            sprintf(line, "%d\n", int1 + int2);
            n = strlen(line);  // 获取结果字符串的长度
            // 将结果字符串写入标准输出
            if (write(STDOUT_FILENO, line, n) != n)
                err_sys("write error");  // 如果写入失败,调用err_sys函数报告错误
        }
        else
        {
            // 如果未能解析两个整数,向标准输出写入错误信息
            if (write(STDOUT_FILENO, "invalid args\n", 13) != 13)
                err_sys("write error");  // 如果写入失败,调用err_sys函数报告错误
        }
    }
    exit(0);  // 退出程序
}
