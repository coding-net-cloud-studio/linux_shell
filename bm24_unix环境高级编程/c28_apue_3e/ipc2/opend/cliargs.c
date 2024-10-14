#include "opend.h"  // 引入opend.h头文件,可能包含函数声明或其他必要的定义.

/*
 * 此函数由 buf_args() 调用,buf_args() 又由 handle_request() 调用.
 * buf_args() 已经将客户端的缓冲区分割成一个 argv[] 风格的数组,我们现在处理它.
 */
int cli_args(int argc, char **argv)
{
    // 检查参数数量是否为3,且第一个参数是否为 CL_OPEN
    if (argc != 3 || strcmp(argv[0], CL_OPEN) != 0)
    {
        // 如果不是,复制错误信息并返回-1
        strcpy(errmsg, "usage: <pathname> <oflag>\n");
        return (-1);
    }
    // 保存指向要打开的路径名的指针
    pathname = argv[1];
    // 将第二个参数转换为整数并保存为文件打开标志
    oflag = atoi(argv[2]);
    // 返回0表示成功处理参数
    return (0);
}
