#include "apue.h"

// main函数是程序的入口点
int main(void)
{
    // 使用lseek函数尝试在标准输入(STDIN_FILENO)上设置文件指针位置
    // SEEK_CUR表示从当前位置偏移,偏移量为0,即不改变当前位置
    // 如果返回值为-1,表示操作失败
    if (lseek(STDIN_FILENO, 0, SEEK_CUR) == -1)
        // 打印错误信息
        printf("cannot seek\n");
    else
        // 如果操作成功,打印成功信息
        printf("seek OK\n");
    // 退出程序,返回状态码0
    exit(0);
}
