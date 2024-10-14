#include <pwd.h>     // 包含passwd结构体和相关的函数声明
#include <stddef.h>  // 包含NULL定义
#include <string.h>  // 包含字符串比较函数strcmp

/**
 * @brief 根据用户名获取passwd结构体指针
 *
 * @param name 用户名
 * @return struct passwd* 返回对应的passwd结构体指针,如果未找到则返回NULL
 */
struct passwd *
getpwnam(const char *name)
{
    struct passwd *ptr;  // 定义一个passwd结构体指针

    setpwent();  // 打开密码文件
    // 循环读取密码文件中的每一条记录
    while ((ptr = getpwent()) != NULL)
        // 如果用户名匹配,则跳出循环
        if (strcmp(name, ptr->pw_name) == 0)
            break; /* found a match */
    endpwent();    // 关闭密码文件
    return (ptr);  // 返回passwd结构体指针,如果未找到匹配项则为NULL
}
