# 平台抽象层——线程管理

- platform_thread.c

- platform_thread.h

这两个文件是对线程处理进行抽象，也会删减相关的功能，只提供满足上层应用的需求，比如提供上层的创建线程、销毁线程、启动线程、停止线程等操作。

## 数据结构

```c
typedef struct platform_thread {
    pthread_t thread;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} platform_thread_t;
```

封装了 **POSIX** 线程库的一些内容，使用到互斥锁（linux 平台）。

## 外部函数

- 创建线程，指定线程的名字、线程主体、线程的传入参数、线程栈大小、优先级、时间片等，这个封装的接口在 linux 平台并不会全部参数都使用上，但为了兼容其他平台，只好这样子封装。

    ```c
    platform_thread_t *platform_thread_init( const char *name,
                                            void (*entry)(void *),
                                            void * const param,
                                            unsigned int stack_size,
                                            unsigned int priority,
                                            unsigned int tick);
    ```

- 启动调度器（在某些平台需要）。

    ```c
    void platform_thread_startup(platform_thread_t* thread)
    ```

- 停止线程。

    ```c
    void platform_thread_stop(platform_thread_t* thread)
    ```

- 启动线程。

    ```c
    void platform_thread_start(platform_thread_t* thread)
    ```

- 销毁线程。

    ```c
    void platform_thread_destroy(platform_thread_t* thread);
    ```

- 等待线程退出，主要是是工作队列中使用。

    ```c
    void platform_thread_wait_exit(platform_thread_t* thread);
    ```

## 依赖

- `#include <pthread.h>`

**上一篇**：[平台抽象层—时间管理](./platform_timer.md)

**下一篇**：[平台抽象层—互斥锁](./platform_mutex.md)
