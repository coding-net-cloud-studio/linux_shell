#include "apue.h"

int main(void)
{
    int c;
    /**
     * 从标准输入中读取字符,直到遇到文件结束符EOF
     */
    while ((c = getc(stdin)) != EOF)
    {
        /**
         * 将读取到的字符写入标准输出
         * @param c 要写入的字符
         * @param stream 目标输出流,此处为标准输出stdout
         * @return 成功返回写入的字符,失败返回EOF
         */
        if (putc(c, stdout) == EOF)
            /**
             * 输出错误,调用err_sys函数打印错误信息并退出程序
             * @param msg 错误信息字符串
             */
            err_sys("output error");
    }
    /**
     * 检查标准输入流是否发生错误
     */
    if (ferror(stdin))
        /**
         * 输入错误,调用err_sys函数打印错误信息并退出程序
         * @param msg 错误信息字符串
         */
        err_sys("input error");
    // 正常退出程序
    exit(0);
}
