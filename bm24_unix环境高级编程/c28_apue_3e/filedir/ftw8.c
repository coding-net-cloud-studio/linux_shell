#include "apue.h"    // 引入apue库,提供系统编程接口
#include <dirent.h>  // 引入dirent库,提供目录操作函数
#include <limits.h>  // 引入limits库,定义了各种数据类型的限制

// 以下代码段没有具体的函数方法,因此无法添加具体的doc注释.
// 通常,对于每个函数,我们会在这里添加描述函数功能,参数和返回值的注释.
// 例如:
/*
 * 函数名: example_function
 * 描述: 这是一个示例函数的注释.
 * 参数:
 *   param1 - 参数1的描述
 *   param2 - 参数2的描述
 * 返回值:
 *   返回值的描述
 */

/* function type that is called for each filename */
// 定义一个函数指针类型Myfunc,它指向一个接受两个const char*和一个int参数并返回int的函数
typedef int Myfunc(const char *, const struct stat *, int);

// 声明一个Myfunc类型的静态函数指针myfunc
static Myfunc myfunc;

// 声明一个静态函数myftw,它接受一个char*和一个Myfunc*参数,并返回一个int
// 这个函数可能用于遍历文件系统,并对每个文件调用传递进来的函数指针
static int myftw(char *, Myfunc *);

// 声明一个静态函数dopath,它接受一个Myfunc*参数,并返回一个int
// 这个函数可能用于处理特定的路径
static int dopath(Myfunc *);

// 定义静态变量用于统计不同类型的文件系统对象的数量
static long nreg,  // 普通文件数量
    ndir,          // 目录数量
    nblk,          // 块设备文件数量
    nchr,          // 字符设备文件数量
    nfifo,         // FIFO(命名管道)数量
    nslink,        // 符号链接数量
    nsock,         // 套接字数量
    ntot;          // 文件系统对象总数量

int main(int argc, char *argv[])
{
    int ret;

    // 检查命令行参数的数量,如果不等于2,则打印使用方法并退出
    if (argc != 2)
        err_quit("usage:  ftw  <starting-pathname>");

    // 调用myftw函数,开始遍历文件系统
    ret = myftw(argv[1], myfunc); /* does it all */

    // 计算所有文件类型的总数,如果没有文件,则设置总数为1以避免除以0
    ntot = nreg + ndir + nblk + nchr + nfifo + nslink + nsock;
    if (ntot == 0)
        ntot = 1; /* avoid divide by 0; print 0 for all counts */

    // 打印各种文件类型的数量及其占总数的百分比
    // 打印常规文件的数量及其占总数的百分比
    printf("常规文件      = %7ld, %5.2f %%\n", nreg, nreg * 100.0 / ntot);

    // 打印目录的数量及其占总数的百分比
    printf("目录          = %7ld, %5.2f %%\n", ndir, ndir * 100.0 / ntot);

    // 打印块特殊文件的数量及其占总数的百分比
    printf("块特殊文件    = %7ld, %5.2f %%\n", nblk, nblk * 100.0 / ntot);

    // 打印字符特殊文件的数量及其占总数的百分比
    printf("字符特殊文件  = %7ld, %5.2f %%\n", nchr, nchr * 100.0 / ntot);

    // 打印FIFOs的数量及其占总数的百分比
    printf("FIFOs         = %7ld, %5.2f %%\n", nfifo, nfifo * 100.0 / ntot);

    // 打印符号链接的数量及其占总数的百分比
    printf("符号链接      = %7ld, %5.2f %%\n", nslink, nslink * 100.0 / ntot);

    // 打印套接字的数量及其占总数的百分比
    printf("套接字        = %7ld, %5.2f %%\n", nsock, nsock * 100.0 / ntot);

    // 退出程序,返回myftw函数的返回值
    exit(ret);
}

/*
 * Descend through the hierarchy, starting at "pathname".
 * 从"pathname"开始遍历层次结构.
 * The caller's func() is called for every file.
 * 对于每个文件,调用调用者的func()函数.
 */
#define FTW_F   1 /* file other than directory */     // 非目录的其他文件
#define FTW_D   2 /* directory */                     // 目录
#define FTW_DNR 3 /* directory that can't be read */  // 无法读取的目录
#define FTW_NS  4 /* file that we can't stat */       // 无法获取状态的文件

static char  *fullpath;  // 包含每个文件的完整路径名
static size_t pathlen;   // 路径名的长度

static int /* 我们返回 func() 的返回值 */
myftw(char *pathname, Myfunc *func)
{
    fullpath = path_alloc(&pathlen); /* 分配 PATH_MAX+1 字节的内存 */
                                     /* ({Prog pathalloc}) */
    // 如果 pathlen 小于等于 pathname 的长度,则重新分配内存
    if (pathlen <= strlen(pathname))
    {
        pathlen = strlen(pathname) * 2;  // 新的长度是 pathname 长度的两倍
        // 尝试重新分配内存
        if ((fullpath = realloc(fullpath, pathlen)) == NULL)
            err_sys("realloc failed");  // 如果失败,报告错误
    }
    strcpy(fullpath, pathname);  // 将 pathname 复制到 fullpath
    return (dopath(func));       // 执行 dopath 函数并返回其结果
}

/*
 * Descend through the hierarchy, starting at "fullpath".
 * If "fullpath" is anything other than a directory, we lstat() it,
 * call func(), and return.  For a directory, we call ourself
 * recursively for each name in the directory.
 */

/*
    从"fullpath"开始遍历层次结构.
    如果"fullpath"不是目录,
    我们就对其执行lstat(),
    调用func(),
    然后返回.
    对于目录,
    我们递归地为目录中的每个名称调用我们自己.
*/

static int /* 我们返回func()的返回值 */
dopath(Myfunc *func)
{
    struct stat    statbuf;  // 用于存储文件状态信息的结构体
    struct dirent *dirp;     // 用于存储目录项信息的结构体指针
    DIR           *dp;       // 目录流指针
    int            ret, n;   // ret用于存储函数返回值,n用于临时存储字符串长度

    // 获取文件状态,如果出错则调用func()并返回其结果
    if (lstat(fullpath, &statbuf) < 0) /* stat error */
        return (func(fullpath, &statbuf, FTW_NS));

    // 检查是否是目录,如果不是则调用func()并返回其结果
    if (S_ISDIR(statbuf.st_mode) == 0) /* not a directory */
        return (func(fullpath, &statbuf, FTW_F));

    // 如果是目录,首先调用func()处理目录本身
    if ((ret = func(fullpath, &statbuf, FTW_D)) != 0)
        return (ret);

    n = strlen(fullpath);  // 获取当前路径字符串长度
    // 如果路径缓冲区不够大,则扩展缓冲区
    // 检查路径缓冲区是否需要扩展
    if (n + NAME_MAX + 2 > pathlen)
    {
        /* 扩展路径缓冲区 */
        // 将路径长度加倍
        pathlen *= 2;
        // 尝试重新分配内存
        if ((fullpath = realloc(fullpath, pathlen)) == NULL)
            // 如果重新分配失败,输出错误信息并退出
            err_sys("realloc failed");
    }

    fullpath[n++] = '/';  // 在路径末尾添加斜杠
    fullpath[n]   = 0;    // 确保字符串以空字符结尾

    // 打开目录流,如果失败则调用func()并返回其结果
    if ((dp = opendir(fullpath)) == NULL) /* can't read directory */
        return (func(fullpath, &statbuf, FTW_DNR));

    // 读取目录中的每个条目
    while ((dirp = readdir(dp)) != NULL)
    {
        // 忽略当前目录和上级目录
        if (strcmp(dirp->d_name, ".") == 0 ||
            strcmp(dirp->d_name, "..") == 0)
            continue;

        // 将目录项名称追加到路径字符串中
        strcpy(&fullpath[n], dirp->d_name);
        // 递归调用dopath()处理子项
        if ((ret = dopath(func)) != 0)
            break;  // 如果返回值不为0,则跳出循环
    }
    fullpath[n - 1] = 0;  // 清除从斜杠开始的所有内容

    // 关闭目录流,如果失败则记录错误
    if (closedir(dp) < 0)
        err_ret("can't close directory %s", fullpath);

    return (ret);  // 返回最终的函数结果
}

static int
myfunc(const char *pathname, const struct stat *statptr, int type)
{
    // 根据文件类型执行不同的操作
    switch (type)
    {
    case FTW_F:  // 普通文件
        // 检查文件模式并相应地增加计数器
        switch (statptr->st_mode & S_IFMT)
        {
        case S_IFREG:  // 普通文件
            nreg++;
            break;
        case S_IFBLK:  // 块设备文件
            nblk++;
            break;
        case S_IFCHR:  // 字符设备文件
            nchr++;
            break;
        case S_IFIFO:  // FIFO文件
            nfifo++;
            break;
        case S_IFLNK:  // 符号链接
            nslink++;
            break;
        case S_IFSOCK:  // 套接字文件
            nsock++;
            break;
        case S_IFDIR:  // 目录,不应该出现在FTW_F类型中
            err_dump("for S_IFDIR for %s", pathname);
        }
        break;
    case FTW_D:  // 目录
        ndir++;
        break;
    case FTW_DNR:  // 无法读取目录
        err_ret("can't read directory %s", pathname);
        break;
    case FTW_NS:  // 文件状态错误
        err_ret("stat error for %s", pathname);
        break;
    default:  // 未知类型
        err_dump("unknown type %d for pathname %s", type, pathname);
    }
    return (0);
}
