# 平台抽象层——互斥锁

- platform_mutex.c

- platform_mutex.h

根据不同平台的处理抽象不同的 **互斥锁** 数据结构、处理函数，进而满足对上层统一的互斥管理操作。

## 数据结构

```c
typedef struct platform_mutex {
    pthread_mutex_t mutex;
} platform_mutex_t;
```

## 外部函数

- 互斥锁的初始化。

    ```c
    int platform_mutex_init(platform_mutex_t* m)
    ```

- 获取互斥锁，当互斥锁被占用时阻塞。

    ```c
    int platform_mutex_lock(platform_mutex_t* m)
    ```

- 尝试获取互斥锁，当互斥锁被占用时不阻塞。

    ```c
    int platform_mutex_trylock(platform_mutex_t* m)
    ```

- 释放互斥锁，只有获取了互斥锁才能释放。

    ```c
    int platform_mutex_unlock(platform_mutex_t* m)
    ```

- 销毁互斥锁。

    ```c
    int platform_mutex_destroy(platform_mutex_t* m)
    ```

## 依赖

- `#include <pthread.h>`

**上一篇**：[平台抽象层—线程管理](./platform_thread.md)

**下一篇**：[平台抽象层—socket](./platform_net_socket.md)
