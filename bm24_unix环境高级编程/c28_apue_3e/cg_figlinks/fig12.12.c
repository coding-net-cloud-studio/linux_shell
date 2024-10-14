#include <string.h>   // 包含字符串操作的库
#include <errno.h>    // 包含错误处理的库
#include <pthread.h>  // 包含POSIX线程(pthreads)的库
#include <stdlib.h>   // 包含标准库函数的库,如malloc和free

// 引入环境变量指针数组
extern char **environ;

// 定义一个递归互斥锁,用于保护环境变量的访问
pthread_mutex_t env_mutex;

// 定义一个静态的pthread_once_t变量,用于确保thread_init函数只被调用一次
static pthread_once_t init_done = PTHREAD_ONCE_INIT;

/**
 * @brief 初始化互斥锁的函数
 *
 * 该函数用于初始化一个递归互斥锁,确保在多线程环境下对环境变量的访问是线程安全的.
 */
static void
thread_init(void)
{
    pthread_mutexattr_t attr;  // 定义互斥锁属性变量

    // 初始化互斥锁属性
    pthread_mutexattr_init(&attr);
    // 设置互斥锁属性为递归类型
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    // 使用设置好的属性初始化互斥锁
    pthread_mutex_init(&env_mutex, &attr);
    // 销毁互斥锁属性变量,释放资源
    pthread_mutexattr_destroy(&attr);
}

// getenv_r 函数用于获取环境变量的值,安全且可重入
// 参数:
//   name - 环境变量的名称
//   buf - 用于存储环境变量值的缓冲区
//   buflen - 缓冲区的长度
// 返回值:
//   0 - 成功获取环境变量值
//   ENOENT - 环境变量不存在
//   ENOSPC - 缓冲区空间不足
int getenv_r(const char *name, char *buf, int buflen)
{
    int i, len, olen;  // 初始化变量

    pthread_once(&init_done, thread_init);  // 确保线程初始化只执行一次
    len = strlen(name);                     // 获取环境变量名称的长度
    pthread_mutex_lock(&env_mutex);         // 加锁,保证线程安全

    // 遍历环境变量数组
    for (i = 0; environ[i] != NULL; i++)
    {
        // 检查当前环境变量是否匹配
        if ((strncmp(name, environ[i], len) == 0) &&
            (environ[i][len] == '='))
        {
            olen = strlen(&environ[i][len + 1]);  // 获取环境变量值的长度
            // 检查缓冲区是否足够存储环境变量值
            if (olen >= buflen)
            {
                pthread_mutex_unlock(&env_mutex);  // 解锁
                return (ENOSPC);                   // 缓冲区空间不足
            }
            strcpy(buf, &environ[i][len + 1]);  // 复制环境变量值到缓冲区
            pthread_mutex_unlock(&env_mutex);   // 解锁
            return (0);                         // 成功获取环境变量值
        }
    }
    pthread_mutex_unlock(&env_mutex);  // 解锁
    return (ENOENT);                   // 环境变量不存在
}
