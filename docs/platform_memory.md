# 平台抽象层——内存管理

- platform_memory.c

- platform_memory.h

根据不同平台的处理抽象不同的处理，进而满足对上层统一的内存管理操作。

## 外部函数

- 动态分配 **size** 大小的内存。

    ```c
    void *platform_memory_alloc(size_t size)
    ```

- 分配内存。

    ```c
    void *platform_memory_calloc(size_t num, size_t size)`
    ```

- 重新追加分配内存。

    ```c
    void *platform_memory_realloc(void *ptr, size_t size)
    ```

- 释放动态申请的内存。

    ```c
    void platform_memory_free(void *ptr)
    ```

**上一篇**：[开发计划](./plan.md)

**下一篇**：[平台抽象层—时间管理](./platform_timer.md)
