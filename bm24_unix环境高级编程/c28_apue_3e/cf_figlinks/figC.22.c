#include <stdio.h>     // 引入标准输入输出库
#include <stdlib.h>    // 引入标准库,提供内存分配和程序控制函数
#include <inttypes.h>  // 引入整型数据类型定义,支持固定宽度的整数类型

/**
 * @brief 主函数,程序的入口点
 *
 * 这个程序会检查当前系统的字节序,并打印出结果.
 *
 * @return int 程序的退出状态码
 */
int main(void)
{
    // 定义一个无符号 32 位整数 i,并初始化为 0x04030201
    uint32_t i = 0x04030201;
    // 定义一个无符号字符指针 cp,指向整数 i 的地址
    unsigned char *cp = (unsigned char *)&i;

    // 如果 cp 指向的字节的值为 1,则说明系统是小端字节序
    if (*cp == 1)
        printf("little-endian\n");
    // 如果 cp 指向的字节的值为 4,则说明系统是大端字节序
    else if (*cp == 4)
        printf("big-endian\n");
    // 如果 cp 指向的字节的值既不是 1 也不是 4,则说明系统的字节序未知
    else
        printf("who knows?\n");
    // 程序正常结束,返回 0
    exit(0);
}
