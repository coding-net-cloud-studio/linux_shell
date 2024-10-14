#include "apue.h"

// main 函数是程序的入口点
int main(void)
{
    // 打印当前进程的ID,%ld 是 long 类型的占位符,(long)getpid() 将进程ID转换为 long 类型
    // getpid() 函数用于获取当前进程的ID
    printf("hello world from process ID %ld\n", (long)getpid());

    // 退出程序,exit(0) 表示程序正常退出
    exit(0);
}
