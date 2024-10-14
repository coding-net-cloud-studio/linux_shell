#include "apue.h"     // 引入APUE库头文件
#include <termios.h>  // 引入终端控制头文件

// 根据不同的操作系统定义不同的选项字符串
#ifdef LINUX
#define OPTSTR "+d:einv"  // LINUX系统下的选项字符串
#else
#define OPTSTR "d:einv"  // 其他系统下的选项字符串
#endif

// 函数声明:设置终端无回显模式
static void set_noecho(int);

// 函数声明:执行驱动程序
// 注意:此函数实现在driver.c文件中
void do_driver(char *);

// 函数声明:循环处理
// 注意:此函数实现在loop.c文件中
void loop(int, int);

int main(int argc, char *argv[])
{
    // 定义文件描述符fdm,字符变量c,以及几个标志变量
    int fdm, c, ignoreeof, interactive, noecho, verbose;  // 初始化变量,用于控制程序行为

    // 定义进程ID变量pid和指向驱动程序名称的指针driver
    pid_t pid;     // 进程ID
    char *driver;  // 驱动程序名称指针

    // 定义从设备名称数组slave_name和原始终端属性结构体orig_termios
    char           slave_name[20];  // 从设备名称
    struct termios orig_termios;    // 原始终端属性

    // 定义窗口大小结构体size
    struct winsize size;  // 窗口大小

    // 检查标准输入是否为终端,如果是,则interactive设置为1
    interactive = isatty(STDIN_FILENO);  // 判断是否为交互式终端

    // 初始化其他标志变量
    ignoreeof = 0;     // 忽略EOF标志
    noecho    = 0;     // 不回显输入标志
    verbose   = 0;     // 详细模式标志
    driver    = NULL;  // 驱动程序名称指针初始化为NULL

    // 设置opterr为0,这样getopt()不会向stderr写入错误信息
    opterr = 0;

    // 循环处理命令行参数,直到没有更多的选项
    while ((c = getopt(argc, argv, OPTSTR)) != EOF)
    {
        // 根据getopt()返回的选项字符进行相应的处理
        switch (c)
        {
        case 'd':  // 设置驱动为stdin/stdout
            driver = optarg;
            break;

        case 'e':  // 对从属pty的行纪律不回显
            noecho = 1;
            break;

        case 'i':  // 忽略标准输入上的EOF
            ignoreeof = 1;
            break;

        case 'n':  // 非交互模式
            interactive = 0;
            break;

        case 'v':  // 详细模式
            verbose = 1;
            break;

        case '?':  // 未识别的选项
            err_quit("unrecognized option: -%c", optopt);
        }
    }

    // 检查命令行参数的数量,如果少于预期,则打印使用方法并退出程序
    if (optind >= argc)
        err_quit("usage: pty [ -d driver -einv ] program [ arg ... ]");

    // 如果程序运行在交互模式下
    if (interactive)
    {
        // 获取当前终端的属性和窗口大小
        if (tcgetattr(STDIN_FILENO, &orig_termios) < 0)          // 获取标准输入的终端属性
            err_sys("tcgetattr error on stdin");                 // 如果出错,打印错误信息并退出
        if (ioctl(STDIN_FILENO, TIOCGWINSZ, (char *)&size) < 0)  // 获取窗口大小
            err_sys("TIOCGWINSZ error");                         // 如果出错,打印错误信息并退出
        // 创建一个伪终端,并传递终端属性和窗口大小
        pid = pty_fork(&fdm, slave_name, sizeof(slave_name), &orig_termios, &size);
    }
    else
    {
        // 如果不是交互模式,创建伪终端但不传递终端属性和窗口大小
        pid = pty_fork(&fdm, slave_name, sizeof(slave_name), NULL, NULL);
    }

    // main.c

    // 如果pid小于0,表示fork操作失败
    if (pid < 0)
    {
        err_sys("fork error");  // 输出错误信息
    }
    // 如果pid等于0,表示当前代码在子进程中运行
    else if (pid == 0)
    { /* child */
        // 如果noecho标志为真,则设置标准输入为无回显模式
        if (noecho)
            set_noecho(STDIN_FILENO); /* stdin是slave pty */

        // 使用execvp函数执行从命令行参数中获取的程序
        // 如果execvp返回值小于0,表示执行失败
        if (execvp(argv[optind], &argv[optind]) < 0)
            err_sys("can't execute: %s", argv[optind]);  // 输出错误信息
    }

    if (verbose)  // 如果设置了详细模式
    {
        fprintf(stderr, "slave name = %s\n", slave_name);  // 打印从设备名称
        if (driver != NULL)                                // 如果驱动程序不为空
            fprintf(stderr, "driver = %s\n", driver);      // 打印驱动程序名称
    }

    if (interactive && driver == NULL)  // 如果是交互模式且没有驱动程序
    {
        if (tty_raw(STDIN_FILENO) < 0)  // 将用户的tty设置为原始模式
            err_sys("tty_raw error");   // 如果出错,打印错误信息
        if (atexit(tty_atexit) < 0)     // 注册tty_atexit函数,用于在退出时重置tty
            err_sys("atexit error");    // 如果出错,打印错误信息
    }

    if (driver)             // 如果有驱动程序
        do_driver(driver);  // 执行驱动程序,可能会改变stdin/stdout

    loop(fdm, ignoreeof);  // 循环复制stdin到ptym,ptym到stdout

    exit(0);  // 程序正常退出
}

static void
set_noecho(int fd) /* 关闭回显(用于从属pty) */
{
    struct termios stermios;  // 定义一个termios结构体变量,用于存储终端属性

    if (tcgetattr(fd, &stermios) < 0)  // 获取当前终端属性
        err_sys("tcgetattr error");    // 如果获取失败,打印错误并退出

    stermios.c_lflag &= ~(ECHO | ECHOE | ECHOK | ECHONL);  // 关闭回显功能

    /*
     * 同时关闭输出时的NL到CR/NL映射.
     */
    stermios.c_oflag &= ~(ONLCR);  // 关闭输出时的换行符转换

    if (tcsetattr(fd, TCSANOW, &stermios) < 0)  // 设置新的终端属性
        err_sys("tcsetattr error");             // 如果设置失败,打印错误并退出
}
