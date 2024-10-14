#include "apue.h"
#include <setjmp.h>

#define TOK_ADD 5

// 定义一个跳转缓冲区,用于非局部跳转
jmp_buf jmpbuffer;

int main(void)
{
    char line[MAXLINE];

    // 设置跳转点,如果从longjmp跳转回来,setjmp会返回非0值
    if (setjmp(jmpbuffer) != 0)
        printf("error");  // 如果发生错误,打印错误信息
    while (fgets(line, MAXLINE, stdin) != NULL)
        do_line(line);  // 读取输入并处理每一行
    exit(0);
}

/*
 * 处理添加命令的函数
 */
void cmd_add(void)
{
    int token;

    token = get_token();        // 获取下一个token
    if (token < 0)              /* an error has occurred */
        longjmp(jmpbuffer, 1);  // 如果发生错误,跳转到setjmp设置的点
    /* rest of processing for this command */
    // 处理命令的其余部分
}
