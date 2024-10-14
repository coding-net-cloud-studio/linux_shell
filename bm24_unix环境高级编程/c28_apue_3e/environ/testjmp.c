#include "apue.h"
#include <setjmp.h>

// 函数声明
static void f1(int, int, int, int);  // f1函数声明,接受四个整型参数
static void f2(void);                // f2函数声明,无参数

// 全局变量
static jmp_buf jmpbuffer;  // 用于存储setjmp的状态
static int     globval;    // 全局变量

int main(void)
{
    int          autoval;  // 自动变量
    register int regival;  // 寄存器变量
    volatile int volaval;  // 易失变量
    static int   statval;  // 静态变量

    // 初始化变量
    globval = 1;
    autoval = 2;
    regival = 3;
    volaval = 4;
    statval = 5;

    // 设置跳转点
    if (setjmp(jmpbuffer) != 0)
    {
        // 如果从longjmp返回,则打印变量的值
        // 打印跳转后的全局变量,自动变量,寄存器变量,易失性变量和静态变量的值
        // 使用exit(0)来正常退出程序
        printf("after longjmp:\n");
        printf("globval = %d, autoval = %d, regival = %d,"
               " volaval = %d, statval = %d\n",
               globval,
               autoval,
               regival,
               volaval,
               statval);
        exit(0);
    }

    // 改变变量值
    globval = 95;
    autoval = 96;
    regival = 97;
    volaval = 98;
    statval = 99;

    // 调用f1函数,该函数不会返回
    f1(autoval, regival, volaval, statval);
    exit(0);
}

// f1函数定义
static void
f1(int i, int j, int k, int l)
{
    // 打印f1函数中的变量值
    // 打印当前函数f1中的变量值
    // globval: 全局变量的值
    // autoval: 自动变量的值,这里对应i
    // regival: 寄存器变量的值,这里对应j
    // volaval: 静态变量的值,这里对应k
    // statval: 静态局部变量的值,这里对应l
    printf("in f1():\n");
    printf("globval = %d, autoval = %d, regival = %d,"
           " volaval = %d, statval = %d\n",
           globval,
           i,
           j,
           k,
           l);

    // 调用f2函数
    f2();
}

// f2函数定义
static void
f2(void)
{
    // 从f2函数跳转回setjmp的位置
    longjmp(jmpbuffer, 1);
}
