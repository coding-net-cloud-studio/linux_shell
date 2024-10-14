#include "apue.h"

// @brief 主函数,测试对标准输入(STDIN_FILENO)的查找操作
int main(void)
{
    // @brief 使用lseek函数尝试对标准输入进行查找操作
    // @param STDIN_FILENO 标准输入的文件描述符
    // @param 0 查找的偏移量,0表示从当前位置开始查找
    // @param SEEK_CUR 查找的起始位置,SEEK_CUR表示从当前位置开始
    if (lseek(STDIN_FILENO, 0, SEEK_CUR) == -1)
        // @brief 如果lseek返回-1,表示查找失败,打印错误信息
        printf("cannot seek\n");
    else
        // @brief 如果lseek返回非-1值,表示查找成功,打印成功信息
        printf("seek OK\n");
    // @brief 退出程序,返回状态码0
    exit(0);
}
