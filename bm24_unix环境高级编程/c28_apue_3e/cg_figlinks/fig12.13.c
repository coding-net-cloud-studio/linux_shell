#include <limits.h>   // 包含整数类型的极限值定义
#include <string.h>   // 包含字符串操作的函数
#include <pthread.h>  // 包含POSIX线程库的定义
#include <stdlib.h>   // 包含通用工具函数的定义

// 定义最大字符串大小
#define MAXSTRINGSZ 4096

// 定义线程特定数据键
static pthread_key_t key;
// 定义一次初始化标志
static pthread_once_t init_done = PTHREAD_ONCE_INIT;
// 定义环境变量的互斥锁
pthread_mutex_t env_mutex = PTHREAD_MUTEX_INITIALIZER;

// 声明外部环境变量数组指针
extern char **environ;

/**
 * @brief 线程初始化函数
 *
 * 该函数用于创建线程特定数据键,并关联释放函数.
 */
static void
thread_init(void)
{
    // 创建线程特定数据键,当线程退出时,使用free函数释放关联的数据
    pthread_key_create(&key, free);
}

// getenv 函数用于获取环境变量的值
// 参数:
//   name: 环境变量的名称
// 返回值:
//   如果找到对应的环境变量,则返回其值的指针;否则返回 NULL
char *
getenv(const char *name)
{
    int   i, len;  // i 用于遍历环境变量数组,len 用于存储环境变量名称的长度
    char *envbuf;  // envbuf 用于存储找到的环境变量的值

    // 使用 pthread_once 确保 thread_init 函数只被调用一次
    pthread_once(&init_done, thread_init);
    // 加锁,保证线程安全
    pthread_mutex_lock(&env_mutex);

    // 获取当前线程特定的环境变量缓冲区
    envbuf = (char *)pthread_getspecific(key);
    // 如果缓冲区为空,则分配内存
    if (envbuf == NULL)
    {
        envbuf = malloc(MAXSTRINGSZ);
        // 内存分配失败,解锁并返回 NULL
        if (envbuf == NULL)
        {
            pthread_mutex_unlock(&env_mutex);
            return (NULL);
        }
        // 设置当前线程特定的环境变量缓冲区
        pthread_setspecific(key, envbuf);
    }

    // 计算环境变量名称的长度
    len = strlen(name);
    // 遍历环境变量数组
    for (i = 0; environ[i] != NULL; i++)
    {
        // 检查当前环境变量是否匹配
        if ((strncmp(name, environ[i], len) == 0) &&
            (environ[i][len] == '='))
        {
            // 复制环境变量的值到缓冲区
            strncpy(envbuf, &environ[i][len + 1], MAXSTRINGSZ - 1);
            // 解锁并返回环境变量的值
            pthread_mutex_unlock(&env_mutex);
            return (envbuf);
        }
    }
    // 如果没有找到匹配的环境变量,解锁并返回 NULL
    pthread_mutex_unlock(&env_mutex);
    return (NULL);
}
