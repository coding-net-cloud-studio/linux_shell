#include <stdio.h>

// 函数:open_data
// 功能:打开名为"datafile"的文件用于读取,并设置其缓冲区
// 返回:成功打开并设置缓冲区后返回文件指针,否则返回NULL
FILE *
open_data(void)
{
    FILE *fp;               // 文件指针
    char  databuf[BUFSIZ];  // 缓冲区,用于存储文件数据

    // 尝试打开文件"datafile"用于读取
    if ((fp = fopen("datafile", "r")) == NULL)
        return (NULL);  // 打开失败,返回NULL

    // 设置文件指针fp的缓冲区为databuf,使用_IOLBF模式(行缓冲),缓冲区大小为BUFSIZ
    if (setvbuf(fp, databuf, _IOLBF, BUFSIZ) != 0)
        return (NULL);  // 设置缓冲区失败,返回NULL

    return (fp);  // 成功打开并设置缓冲区,返回文件指针
}
