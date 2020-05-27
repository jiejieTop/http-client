# 平台抽象层——时间管理

- platform_timer.c

- platform_timer.h

这两个文件只是为了获取平台的时间，为上层应用提供时基、超时检测、更新时间等功能。

## 数据结构

```c
typedef struct platform_timer {
    struct timeval time;
} platform_timer_t;
```

封装了平台层的时间类型的变量（数据结构）。

## 外部函数

- 时间初始化，初始化时间这个数据结构，准备开始计时。

    ```c
    void platform_timer_init(platform_timer_t* timer)
    ```

- 开始倒计时，超时的时间是 **timeout**。

    ```c
    void platform_timer_cutdown(platform_timer_t* timer, unsigned int timeout)
    ```

- 判断是否超时。

    ```c
    char platform_timer_is_expired(platform_timer_t* timer)
    ```

- 获取距离超时还剩多少时间。

    ```c
    int platform_timer_remain(platform_timer_t* timer)
    ```

- 获取平台当前的时间。

    ```c
    unsigned long platform_timer_now(void)
    ```

- 进行 us 的延时

    ```c
    void platform_timer_usleep(unsigned long usec)
    ```

**上一篇**：[平台抽象层—内存管理](./platform_memory.md)

**下一篇**：[平台抽象层—线程管理](./platform_thread.md)
