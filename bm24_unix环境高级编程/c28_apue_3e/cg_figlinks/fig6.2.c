#include <pwd.h>     // 包含用户信息结构体的头文件
#include <stddef.h>  // 包含标准定义的头文件,如NULL
#include <string.h>  // 包含字符串操作函数的头文件

// @brief 根据用户名获取用户信息的函数
// @param name 用户名
// @return 如果找到匹配的用户信息,则返回指向passwd结构体的指针;如果没有找到,则返回NULL
struct passwd *
getpwnam(const char *name)
{
    struct passwd *ptr;  // 用于存储从getpwent()获取的用户信息的指针

    setpwent();                                        // 打开密码文件
    while ((ptr = getpwent()) != NULL)                 // 逐行读取密码文件
        if (strcmp(name, ptr->pw_name) == 0)           // 如果找到匹配的用户名
            break; /* found a match */                 // 跳出循环
    endpwent();                                        // 关闭密码文件
    return (ptr); /* ptr is NULL if no match found */  // 返回用户信息指针或NULL
}
