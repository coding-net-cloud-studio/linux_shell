#include <stdio.h>     // 引入标准输入输出库,用于执行输入输出操作
#include <stdlib.h>    // 引入标准库,提供一些通用功能,如内存分配和程序终止
#include <inttypes.h>  // 引入整型数据类型库,定义了一组固定宽度的整数类型

// 主函数,用于检测系统的字节序
int main(void)
{
    // 定义一个32位无符号整数,其二进制表示按照大端序存储
    uint32_t i = 0x04030201;
    // 定义一个无符号字符指针,指向整数i的地址
    unsigned char *cp = (unsigned char *)&i;

    // 检查最低有效字节(little-endian)是否为1
    if (*cp == 1)
        printf("little-endian\n");  // 如果是,打印小端序
    // 检查最高有效字节(big-endian)是否为4
    else if (*cp == 4)
        printf("big-endian\n");  // 如果是,打印大端序
    else
        printf("who knows?\n");  // 如果都不是,打印未知
    exit(0);                     // 退出程序
}
