#include "apue.h"

// 主函数
int main(void)
{
    // 打印实际用户ID和有效用户ID
    printf("real uid = %d, effective uid = %d\n", getuid(), geteuid());
    // 退出程序,返回状态码0
    exit(0);
}
