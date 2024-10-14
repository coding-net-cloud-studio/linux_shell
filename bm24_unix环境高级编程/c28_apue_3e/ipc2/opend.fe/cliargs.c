#include "opend.h"  // 包含头文件,可能定义了与打开文件相关的函数和数据结构.

/*
 * This function is called by buf_args(), which is called by
 * handle_request().  buf_args() has broken up the client's
 * buffer into an argv[]-style array, which we now process.
 */

/*
 * 此函数由buf_args()调用,buf_args()又由handle_request()调用.
 * buf_args()已经将客户端的缓冲区分割成一个argv[]风格的数组,我们现在处理这个数组.
 */
int cli_args(int argc, char **argv)
{
    // 检查参数数量是否为3,且第一个参数是否为CL_OPEN
    // 检查命令行参数的数量是否为3,且第一个参数是否为CL_OPEN
    // 如果不是,将错误信息复制到errmsg并返回-1
    if (argc != 3 || strcmp(argv[0], CL_OPEN) != 0)
    {
        // 如果不是,复制错误信息到errmsg并返回-1
        strcpy(errmsg, "usage: <pathname> <oflag>\n");
        return (-1);
    }

    // 保存指向要打开的路径名的指针
    pathname = argv[1];
    // 将第二个参数转换为整数并保存到oflag
    oflag = atoi(argv[2]);
    // 返回0表示成功
    return (0);
}
