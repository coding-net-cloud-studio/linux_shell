#include "opend.h"  // 引入opend.h头文件,该文件可能包含了程序所需的函数声明和定义.

/*
 * This function is called by buf_args(), which is called by
 * handle_request().  buf_args() has broken up the client's
 * buffer into an argv[]-style array, which we now process.
 */

/*
 * 此函数由buf_args()调用,buf_args()又由handle_request()调用.
 * buf_args()已经将客户端的缓冲区分割成一个argv[]风格的数组,我们现在处理它.
 */

// cli_args函数用于处理客户端传递的参数,确保参数数量和格式正确.
// 参数:
//   argc: 参数的数量
//   argv: 参数的数组,其中argv[0]是命令名称,argv[1]是路径名,argv[2]是打开标志
// 返回值:
//   如果参数数量不为3或者命令名称不是CL_OPEN,则返回-1,并设置错误信息.
//   否则,保存路径名和打开标志,并返回0.
int cli_args(int argc, char **argv)
{
    // 检查参数数量是否为3,且第一个参数是否为CL_OPEN
    if (argc != 3 || strcmp(argv[0], CL_OPEN) != 0)
    {
        // 如果参数不正确,复制错误信息到errmsg
        strcpy(errmsg, "usage: <pathname> <oflag>\n");
        return (-1);  // 返回错误代码
    }
    pathname = argv[1];        // 保存指向要打开的路径名的指针
    oflag    = atoi(argv[2]);  // 将第二个参数转换为整数并保存为打开标志
    return (0);                // 参数正确,返回成功代码
}
