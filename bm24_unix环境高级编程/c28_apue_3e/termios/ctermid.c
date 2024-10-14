#include <stdio.h>   // 包含标准输入输出库,用于文件操作和打印输出
#include <string.h>  // 包含字符串处理库,用于字符串操作

// 定义一个静态字符数组,用于存储终端名称
static char ctermid_name[L_ctermid];

/**
 * ctermid - 获取当前终端的设备名称
 *
 * @str: 如果不为NULL,则将设备名称复制到此字符串中;如果为NULL,则使用静态字符数组
 *
 * 返回值: 指向包含终端设备名称的字符串的指针
 */
char *
ctermid(char *str)
{
    // 如果传入的str为NULL,则使用静态字符数组作为目标字符串
    if (str == NULL)
        str = ctermid_name;
    // 将终端设备名称"/dev/tty"复制到str指向的内存中,并返回str
    return (strcpy(str, "/dev/tty")); /* strcpy() returns str */
}
