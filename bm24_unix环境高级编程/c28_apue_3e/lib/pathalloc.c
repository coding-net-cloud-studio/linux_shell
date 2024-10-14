#include "apue.h"    // 引入APUE库的头文件,该库提供了一套用于编写可移植UNIX应用程序的函数.
#include <errno.h>   // 引入错误处理头文件,定义了错误码.
#include <limits.h>  // 引入定义各种数据类型限制的头文件.

// 如果定义了PATH_MAX,则pathmax为PATH_MAX的值,否则为0
// 如果定义了PATH_MAX,则pathmax的值为PATH_MAX的值
// 否则,pathmax的值为0
#ifdef PATH_MAX
static long pathmax = PATH_MAX;
#else
static long pathmax = 0;
#endif

// 初始化posix和xsi版本号
static long posix_version = 0;
static long xsi_version   = 0;

// 如果PATH_MAX不确定,使用这个猜测值
#define PATH_MAX_GUESS 1024

/**
 * path_alloc - 分配足够的内存来存储路径名
 * @sizep: 如果非空,返回分配的大小
 *
 * 返回值: 分配的内存块的指针,如果出错则为NULL
 */
char *
path_alloc(size_t *sizep) /* also return allocated size, if nonnull */
{
    char  *ptr;   // 指向分配内存的指针
    size_t size;  // 分配的内存大小

    // 获取当前的posix版本号
    // 如果posix_version为0,则通过sysconf函数获取系统的POSIX版本号并赋值给posix_version
    if (posix_version == 0)
        posix_version = sysconf(_SC_VERSION);

    // 获取当前的xsi版本号
    // 如果xsi_version为0,则通过sysconf函数获取系统的XSI版本号并赋值给xsi_version
    if (xsi_version == 0)
        xsi_version = sysconf(_SC_XOPEN_VERSION);

    // 如果pathmax未设置,则尝试获取PATH_MAX的值

    // 如果pathmax为0,表示首次执行此段代码
    if (pathmax == 0)
    {
        // 将errno清零,用于后续错误检查
        errno = 0;

        // 使用pathconf函数获取根目录"/"下的最大路径长度
        if ((pathmax = pathconf("/", _PC_PATH_MAX)) < 0)
        {
            // 如果errno未被设置,说明PATH_MAX的值不确定,使用预设的猜测值
            if (errno == 0)
                pathmax = PATH_MAX_GUESS;  // 使用猜测值

            // 如果errno被设置,说明pathconf调用出错,进行错误处理
            else
                err_sys("pathconf error for _PC_PATH_MAX");
        }
        else
        {
            // 如果成功获取到PATH_MAX的值,由于它是相对于根目录的,所以需要加1
            pathmax++;
        }
    }

    // 根据posix和xsi版本号确定路径名的大小
    if ((posix_version < 200112L) && (xsi_version < 4))
        size = pathmax + 1;  // 旧版本需要为终止null字节额外分配空间
    else
        size = pathmax;

    // 分配内存
    if ((ptr = malloc(size)) == NULL)
        err_sys("malloc error for pathname");  // 内存分配失败处理

    // 如果sizep非空,返回分配的大小
    if (sizep != NULL)
        *sizep = size;
    return (ptr);  // 返回分配的内存块指针
}
