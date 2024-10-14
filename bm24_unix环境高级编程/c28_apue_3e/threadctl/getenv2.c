#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <stdlib.h>

// 引用全局环境变量指针数组
extern char **environ;

// 定义一个互斥锁,用于保护环境变量的访问
pthread_mutex_t env_mutex;

// 定义一个一次性初始化控制结构体
static pthread_once_t init_done = PTHREAD_ONCE_INIT;

// 初始化互斥锁的函数
// 该函数只会被调用一次,确保互斥锁的正确初始化
static void
thread_init(void)
{
    pthread_mutexattr_t attr;

    // 初始化互斥锁属性
    pthread_mutexattr_init(&attr);
    // 设置互斥锁类型为递归锁
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    // 使用属性初始化互斥锁
    pthread_mutex_init(&env_mutex, &attr);
    // 销毁互斥锁属性对象
    pthread_mutexattr_destroy(&attr);
}

/**
 * getenv_r - 安全版本的getenv函数,用于多线程环境
 * @name: 环境变量的名称
 * @buf: 用于存储环境变量值的缓冲区
 * @buflen: 缓冲区的大小
 *
 * 返回值: 成功返回0,缓冲区太小返回ENOSPC,未找到环境变量返回ENOENT
 */
int getenv_r(const char *name, char *buf, int buflen)
{
    int i, len, olen;

    // 确保互斥锁只被初始化一次
    pthread_once(&init_done, thread_init);
    // 获取环境变量名称的长度
    len = strlen(name);
    // 加锁,保护环境变量的访问
    pthread_mutex_lock(&env_mutex);
    // 遍历环境变量数组
    for (i = 0; environ[i] != NULL; i++)
    {
        // 检查当前环境变量是否匹配请求的名称
        if ((strncmp(name, environ[i], len) == 0) &&
            (environ[i][len] == '='))
        {
            // 获取环境变量值的长度
            olen = strlen(&environ[i][len + 1]);
            // 检查缓冲区是否足够大
            if (olen >= buflen)
            {
                // 缓冲区太小,解锁并返回错误码
                pthread_mutex_unlock(&env_mutex);
                return (ENOSPC);
            }
            // 复制环境变量值到缓冲区
            strcpy(buf, &environ[i][len + 1]);
            // 解锁并返回成功
            pthread_mutex_unlock(&env_mutex);
            return (0);
        }
    }
    // 未找到环境变量,解锁并返回错误码
    pthread_mutex_unlock(&env_mutex);
    return (ENOENT);
}
