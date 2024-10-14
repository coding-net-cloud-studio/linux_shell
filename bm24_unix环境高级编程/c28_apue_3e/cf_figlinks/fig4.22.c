#include "apue.h"
#include <dirent.h>
#include <limits.h>

/* 定义一个函数类型,该函数类型在遍历每个文件名时被调用 */
typedef int Myfunc(const char *, const struct stat *, int);

/* 静态函数myfunc的声明,具体实现未给出 */
static Myfunc myfunc;

/* 静态函数myftw的声明,用于遍历文件系统树 */
static int myftw(char *, Myfunc *);

/* 静态函数dopath的声明,用于处理路径 */
static int dopath(Myfunc *);

/* 定义了一系列静态变量,用于统计不同类型文件的数量 */
// 定义静态变量用于统计不同类型的文件系统对象的数量
// nreg: 普通文件的数量
// ndir: 目录的数量
// nblk: 块设备的数量
// nchr: 字符设备的数量
// nfifo: FIFO (命名管道)的数量
// nslink: 符号链接的数量
// nsock: 套接字的数量
// ntot: 所有类型文件系统对象的总数
static long nreg, ndir, nblk, nchr, nfifo, nslink, nsock, ntot;

int main(int argc, char *argv[])
{
    int ret;

    // 检查命令行参数的数量,如果不等于2,则打印使用方法并退出程序
    if (argc != 2)
        err_quit("usage:  ftw  <starting-pathname>");

    // 调用myftw函数,开始遍历文件系统,并将结果存储在ret中
    ret = myftw(argv[1], myfunc); /* does it all */

    // 计算所有文件类型的总数,如果没有文件,则设置ntot为1以避免除以0的错误
    // 计算所有文件系统对象的总数
    ntot = nreg + ndir + nblk + nchr + nfifo + nslink + nsock;

    // 防止除以零的情况发生;如果总数为0,则将总数设置为1,并打印所有计数为0
    if (ntot == 0)
        ntot = 1; /* avoid divide by 0; print 0 for all counts */

    // 打印各种文件类型的数量和百分比
    // 打印常规文件的数量及其占总数的百分比
    printf("常规文件  = %7ld, %5.2f %%\n", nreg, nreg * 100.0 / ntot);

    // 打印目录的数量及其占总数的百分比
    printf("目录      = %7ld, %5.2f %%\n", ndir, ndir * 100.0 / ntot);

    // 打印块特殊文件的数量及其占总数的百分比
    printf("块特殊文件  = %7ld, %5.2f %%\n", nblk, nblk * 100.0 / ntot);

    // 打印字符特殊文件的数量及其占总数的百分比
    printf("字符特殊文件  = %7ld, %5.2f %%\n", nchr, nchr * 100.0 / ntot);

    // 打印FIFOs的数量及其占总数的百分比
    printf("FIFOs       = %7ld, %5.2f %%\n", nfifo, nfifo * 100.0 / ntot);

    // 打印符号链接的数量及其占总数的百分比
    printf("符号链接   = %7ld, %5.2f %%\n", nslink, nslink * 100.0 / ntot);

    // 打印套接字的数量及其占总数的百分比
    printf("套接字      = %7ld, %5.2f %%\n", nsock, nsock * 100.0 / ntot);

    // 根据ret的值退出程序
    exit(ret);
}

/*
 * Descend through the hierarchy, starting at "pathname".
 * The caller's func() is called for every file.
 */
/*
 * 该函数用于遍历从"pathname"开始的文件系统层次结构.
 * 每遇到一个文件,就会调用调用者提供的func()函数.
 */

/*
 * Descend through the hierarchy, starting at "pathname".
 * 从"pathname"开始遍历层次结构.
 * The caller's func() is called for every file.
 * 对于每个文件,都会调用调用者的func()函数.
 */
#define FTW_F   1 /* file other than directory */     // 定义常量FTW_F表示非目录的其他文件
#define FTW_D   2 /* directory */                     // 定义常量FTW_D表示目录
#define FTW_DNR 3 /* directory that can't be read */  // 定义常量FTW_DNR表示无法读取的目录
#define FTW_NS  4 /* file that we can't stat */       // 定义常量FTW_NS表示我们无法获取状态的文件

static char  *fullpath;  // 定义静态字符指针fullpath,用于存储每个文件的完整路径名
static size_t pathlen;   // 定义静态size_t类型变量pathlen,用于存储路径长度

static int /* 我们返回func()的返回值 */
myftw(char *pathname, Myfunc *func)
{
    fullpath = path_alloc(&pathlen); /* 分配PATH_MAX+1字节的内存 */
                                     /* ({Prog pathalloc}) */
    // 如果pathlen小于等于pathname的长度,则将pathlen设置为pathname长度的两倍
    if (pathlen <= strlen(pathname))
    {
        pathlen = strlen(pathname) * 2;
        // 如果realloc失败,返回NULL并报错
        // 如果需要重新分配fullpath指向的内存空间以适应新的路径长度pathlen,
        // 使用realloc函数.如果realloc失败,返回NULL,此时打印错误信息并退出程序.
        if ((fullpath = realloc(fullpath, pathlen)) == NULL)
            err_sys("realloc失败");
    }
    // 将pathname的内容复制到fullpath
    strcpy(fullpath, pathname);
    // 调用dopath()函数并返回其结果
    return (dopath(func));
}

/*
 * Descend through the hierarchy, starting at "fullpath".
 * If "fullpath" is anything other than a directory, we lstat() it,
 * call func(), and return.  For a directory, we call ourself
 * recursively for each name in the directory.
 */

/*
 * Descend through the hierarchy, starting at "fullpath".
 * 如果"fullpath"不是目录,我们对其执行lstat()操作,
 * 调用func()函数,并返回.对于目录,我们递归地对其
 * 中的每个名称调用自身.
 */

static int /* 返回func()的返回值 */
dopath(Myfunc *func)
{
    struct stat    statbuf;  // 文件状态信息结构体
    struct dirent *dirp;     // 目录项结构体指针
    DIR           *dp;       // 目录流指针
    int            ret, n;

    // 获取文件状态,如果不是目录则直接调用func()处理
    if (lstat(fullpath, &statbuf) < 0) /* stat错误 */
        return (func(fullpath, &statbuf, FTW_NS));
    if (S_ISDIR(statbuf.st_mode) == 0) /* 不是目录 */
        return (func(fullpath, &statbuf, FTW_F));

    // 是目录,先调用func()处理目录本身
    if ((ret = func(fullpath, &statbuf, FTW_D)) != 0)
        return (ret);

    n = strlen(fullpath);
    // 如果路径缓冲区不够大,则扩展缓冲区
    // 检查当前路径长度是否足够,如果不足以存储新的路径名,则重新分配内存
    if (n + NAME_MAX + 2 > pathlen)
    {
        // 将路径长度加倍
        pathlen *= 2;
        // 尝试重新分配内存
        if ((fullpath = realloc(fullpath, pathlen)) == NULL)
            // 如果分配失败,输出错误信息并退出
            err_sys("realloc失败");
    }
    // 在路径后添加斜杠
    fullpath[n++] = '/';
    // 确保字符串以空字符结尾
    fullpath[n] = 0;

    // 打开目录
    // 打开指定路径的目录,如果失败则调用func函数并返回其结果
    // 参数fullpath是要打开的目录的完整路径
    // 参数statbuf是一个指向stat结构的指针,用于存储目录信息(未在片段中使用)
    // 参数FTW_DNR是一个标志,表示遇到不可读取的目录时的行为
    if ((dp = opendir(fullpath)) == NULL)            // 尝试打开目录
        return (func(fullpath, &statbuf, FTW_DNR));  // 如果打开失败,调用func函数并返回结果

    // 遍历目录中的每个文件
    // 读取目录中的每个条目
    while ((dirp = readdir(dp)) != NULL)
    {
        // 忽略当前目录"."和上级目录".."的条目
        if (strcmp(dirp->d_name, ".") == 0 ||
            strcmp(dirp->d_name, "..") == 0)
            continue;  // 继续下一次循环

        // 将读取到的文件名追加到完整路径的末尾
        strcpy(&fullpath[n], dirp->d_name);

        // 递归处理当前文件或子目录
        // 假设dopath是一个已定义的函数,它接受一个函数指针作为参数
        // 并返回一个整数,表示处理的结果
        // 如果返回值不为0,表示遇到了需要中断的情况
        if ((ret = dopath(func)) != 0)
            break;  // 跳出循环
    }

    fullpath[n - 1] = 0;  // 清除斜杠及之后的内容

    // 关闭目录流
    if (closedir(dp) < 0)
        err_ret("无法关闭目录%s", fullpath);
    return (ret);
}

// myfunc 函数用于处理文件路径名,文件状态指针以及文件类型
// 参数:
//   pathname: 文件的路径名
//   statptr: 指向文件状态信息的指针
//   type: 文件类型
static int
myfunc(const char *pathname, const struct stat *statptr, int type)
{
    // 根据文件类型进行不同的处理
    switch (type)
    {
    case FTW_F:  // 普通文件
        // 检查文件模式并统计不同类型的文件数量
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
        case S_IFDIR:                                  // 目录,应该有类型 = FTW_D
            err_dump("for S_IFDIR for %s", pathname);  // 错误处理
        }
        break;
    case FTW_D:  // 目录
        ndir++;  // 统计目录数量
        break;
    case FTW_DNR:                                      // 无法读取目录
        err_ret("can't read directory %s", pathname);  // 错误处理
        break;
    case FTW_NS:                                 // stat错误
        err_ret("stat error for %s", pathname);  // 错误处理
        break;
    default:                                                          // 未知类型
        err_dump("unknown type %d for pathname %s", type, pathname);  // 错误处理
    }
    return (0);
}
