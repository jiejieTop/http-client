# 工作队列

- http_wq.c

- http_wq.h

工作队列是实现异步的手段，用户调用 API 接口后，不会阻塞，而是将后续要处理的函数放入工作队列中，就立即返回，然后工作队列就默默开始工作，根据先后顺序去处理这些工作，处理完之后，通过回调函数将数据上报到应用层，并消耗这个工作。

当然啦，在 linux 平台下，工作队列会根据当前 CPU 支持的最大线程数创建工作的线程，力求并发处理这些工作。

## 数据结构

- 工作队列的任务函数指针原型，所有放入工作队列的任务都要满足下面这种形式。

    ```c
    typedef void (*http_worker_func_t)(void *);
    ```

- 工作队列的数据结构，loop 表示此事工作队列线程需要持续运行，thread 是工作队列的线程指针，mutex 是互斥锁，因为要对 list 进行操作，要包含这个成员变量，list 则是用于挂载工作队列中的所有要处理的工作。

    ```c
    typedef struct http_wq {
        int                 loop;
        platform_thread_t   *thread;
        platform_mutex_t    mutex;
        http_list_t         list;
    } http_wq_t;
    ```

- 工作队列池，所有的工作队列都会被放在工作队列池中，记录了当前系统的 cup 支持的线程数，ring 则是循环将处理的任务放到不同的处理线程中，wq 成员变量指向了实际处理的工作队列空间。

    ```c
    typedef struct http_wq_pool {
        int                 cpus;
        int                 ring;
        http_wq_t           *wq;         //multi workq
    } http_wq_pool_t;
    ```

- 具体的工作，entry 成员变量表示挂载在哪个工作队列中，func 是具体的工作处理函数，data 则是传递给 func 的变量，由用户指定，wq 用于记录自己属于那个工作队列。

    ```c
    typedef struct worker {
        http_list_t         entry;
        http_worker_func_t  func;
        void                *data;
        http_wq_t           *wq;           //root
    } http_worker_t;
    ```

## 外部函数

- 工作队列池的初始化与销毁，初始化的时候会根据 **CPU** 支持的线程数去创建 **一个或者多个线程**，销毁则将所有正在运行的工作队列及其线程都销毁。

    ```c
    int http_wq_pool_init(void);
    void http_wq_pool_deinit(void);
    ```

- 等待工作队列处理完毕并退出，这个函数没有 **http_wq_pool_deinit()** 函数那么粗暴，会等待所有的工作完成后再去销毁工作队列。

    ```c
    void http_wq_wait_exit(void);
    ```

- 添加任务到工作队列中，此时会 **唤醒** 工作队列的线程（如果该线程处于休眠状态的话）。

    ```c
    int http_wq_add_task(http_worker_func_t func, void *data, size_t len);
    ```

## 内部函数

- 工作队列处理线程，如果没有需要处理的工作，线程会处于阻塞状态，等待工作的到来。

    ```c
    static void _http_wq_thread(void *arg);
    ```

- 获取要处理的第一个工作。

    ```c
    static http_worker_t *_http_get_frist_worker(http_wq_t *wq);
    ```

- 创建一个工作并添加到工作队列中。

    ```c
    static int _http_wq_worker_create(http_wq_t *wq, http_worker_func_t func, void *data, size_t len);
    ```

- 销毁一个工作。

    ```c
    static int _http_wq_worker_destroy(http_worker_t *w)
    ```

**上一篇**：[拦截器](./interceptor.md)

**下一篇**：[httpclient](./client.md)
