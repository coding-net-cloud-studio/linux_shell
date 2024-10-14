#include "apue.h"    // 引入apue库的头文件
#include <setjmp.h>  // 引入setjmp库的头文件,用于非局部跳转

// 函数f1的原型声明,接受四个整型参数
/**
 * @brief 函数f1的说明
 * @param 第一个整型参数
 * @param 第二个整型参数
 * @param 第三个整型参数
 * @param 第四个整型参数
 */
static void f1(int, int, int, int);

// 函数f2的原型声明,无参数
/**
 * @brief 函数f2的说明
 */
static void f2(void);

static jmp_buf jmpbuffer;  // 定义一个jmp_buf类型的变量,用于保存非局部跳转的状态
static int     globval;    // 定义一个全局整型变量
int            main(void)
{
    int          autoval;  // 自动变量,函数调用结束后会被销毁
    register int regival;  // 寄存器变量,建议编译器存储在寄存器中以提高访问速度
    volatile int volaval;  // 易失变量,告诉编译器该变量的值可能会在任何时候被外部因素改变
    static int   statval;  // 静态变量,生命周期贯穿整个程序运行期间,只初始化一次

    // globval 是一个全局变量,生命周期贯穿整个程序运行期间,初始值为1
    globval = 1;

    // autoval 是一个自动变量,通常在函数内部声明,生命周期仅限于所在的代码块,初始值为2
    autoval = 2;

    // regival 是一个寄存器变量,存储在CPU的寄存器中,访问速度最快,初始值为3
    regival = 3;

    // volaval 是一个volatile变量,告诉编译器该变量的值可能会被意外改变,需要直接从内存中读取,初始值为4
    volaval = 4;

    // statval 是一个静态变量,生命周期贯穿整个程序运行期间,但作用域限制在声明它的文件内,初始值为5
    statval = 5;

    if (setjmp(jmpbuffer) != 0)  // 设置非局部跳转点,如果之前已经跳转过这里,返回非0值
    {
        // 打印跳转后的变量值
        // 该代码段使用printf函数输出五个变量的值,这些变量分别是全局变量globval,自动变量autoval,寄存器变量regival,易失变量volaval和静态变量statval
        printf("after longjmp:\n");
        printf("globval = %d, autoval = %d, regival = %d,"
               " volaval = %d, statval = %d\n",
               globval,
               autoval,
               regival,
               volaval,
               statval);

        exit(0);  // 结束程序
    }

    /*
     * 在调用setjmp之后,但在调用longjmp之前,更改变量的值.
     * 这段代码用于演示在非局部跳转中如何修改不同类型的变量.
     */
    globval = 95;  // 修改全局变量的值
    autoval = 96;  // 修改自动变量的值
    regival = 97;  // 修改寄存器变量的值(实际上在C语言中无法直接声明寄存器变量)
    volaval = 98;  // 修改易失变量的值(实际上在C语言中没有易失变量这个概念,可能是指volatile关键字修饰的变量)
    statval = 99;  // 修改静态变量的值

    f1(autoval, regival, volaval, statval); /* never returns */  // 调用函数f1,假设该函数内会执行longjmp跳转回setjmp处
    exit(0);                                                     // 如果f1函数正常返回,则结束程序
}

// 函数f1:打印全局变量和局部变量的值,并调用函数f2
// 参数:
//   i - 整数类型,局部变量
//   j - 整数类型,局部变量
//   k - 整数类型,局部变量
//   l - 整数类型,局部变量
static void
f1(int i, int j, int k, int l)
{
    printf("in f1():\n");  // 打印函数f1进入的信息
    // 打印全局变量和局部变量的值
    printf("globval = %d, autoval = %d, regival = %d,"
           " volaval = %d, statval = %d\n",
           globval,
           i,
           j,
           k,
           l);
    f2();  // 调用函数f2
}

// 函数f2:执行longjmp函数,跳转回setjmp的位置
static void
f2(void)
{
    longjmp(jmpbuffer, 1);  // 使用longjmp跳转回setjmp的位置,并返回值1
}
