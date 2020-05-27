# httpclient

这应该是用户最容易接触到的上层应用了，虽然用户可以接触它，但是我并不打算将 client 对外公开操作，除非很熟悉（至少我目前是这样子打算的），因此我将它设置成内部管理。

使用池化技术，将所有的 **client** 都放在一个 **client pool** 中，需要的时候从池中租借（**lease**），在使用完毕归还到 **client pool** 中（**release**）。

## 数据结构

```c
typedef struct http_client {
    http_list_t                         list;
    http_interceptor_t                  *interceptor;
    http_connect_params_t               *connect_params;
    http_request_method_t               method;
    uint32_t                            interest_event;
    http_event_t                        *event;
    size_t                              process;
    size_t                              total;
    void                                *data;
} http_client_t;
```

- list: **http-client** 内部维护了两条链表，分别是 **已使用** 与 **空闲** 链表，而 list 则是用于挂载到这两个列表中的，以及一个用于包含 **client** 池的 **_client_pool_lock** 互斥锁：

    ```c
    static http_list_t _http_client_free_list;
    static http_list_t _http_client_used_list;

    static platform_mutex_t _client_pool_lock;
    ```

- interceptor：拥有的拦截器。

- connect_params：指向连接参数结构。

- method：请求的模式，如 **GET/POST** 等。

- interest_event：感兴趣的事件，在拦截器收到数据的时候触发回调函数，而通过感兴趣的事件选择是否上报到用户空间。

- event：事件回调处理。

- process：已经处理的数据长度。

- total：总数据长度。

- data：用户要发送给服务器的数据（**POST** 的数据）。

## 外部函数

- **http-client** 的初始化，应该称之为系统初始化更合适，它会初始化内部管理的 **client pool**，用链表的形式去管理所有的空闲 **client**，数量由用户指定 **HTTP_CLIENT_POOL_SIZE**，默认为 **5**。

    ```c
    int http_client_init(const char *ca);
    ```

- 等待处理完毕并退出，如果任务还处于工作队列中，本函数将持续等待工作队列的工作完成，当然，如果不关心工作队列中的任务处理结果，可以调用工作队列的销毁函数，直接销毁工作队列并退出。

    ```c
    void http_client_wait_exit(void);
    ```

- 直接退出处理，本函数会销毁工作队列。

    ```c
    void http_client_exit(void);
    ```

- 租借 **client**，其实就是从空闲链表 **_http_client_free_list** 中取出一个 **client**，返回给用户。

    ```c
    http_client_t *http_client_lease(void);
    ```

- **http-client** 释放操作，这应该叫将 **client** 放回空闲链表 **_http_client_free_list** 中。

    ```c
    void http_client_release(http_client_t *c);
    ```

- 用户接口，**http get** 数据，通过回调函数上报数据，它只需 **url 参数** 与对应的回调函数即可，在内部会从空闲链表 **_http_client_free_list** 中分配一个 **client** 用于处理 url，处理完了会释放 **client**。

    ```c
    int http_client_get(const char *url, http_event_cb_t cb);
    ```

- 用户接口，**http post** 数据，指定要 **post** 的 **url** 与数据 **内容**。

    ```c
    int http_client_post(const char *url, void *data ,http_event_cb_t cb);
    ```

- 注册感兴趣的事件，在产生该事件的时候将会 **告知应用层**。

    ```c
    void http_client_set_interest_event(http_client_t *c, http_event_type_t event);
    ```

- 设置 http 的请求方法。

    ```c
    void http_client_set_method(http_client_t *c, http_request_method_t method);
    ```

- 设置 http 要发送给服务器的数据。

    ```c
    void http_client_set_data(http_client_t *c, void *data);
    ```

## 内部函数

- **client pool** 的初始化，其实就是根据指定的 **client** 数量去初始化 **client** 池，并挂载到空闲链表 **_http_client_free_list** 中，在需要的时候从该链表取下。

    ```c
    void *_http_client_pool_init(void)
    ```

- client 的初始化，主要就是分配相关的内存空间。

    ```c
    int _http_client_internal_init(void)
    ```

- 根据用户指定的感兴趣的事件进行回调处理。

    ```c
    static int _http_client_internal_event_handle(void *e)
    ```

**上一篇**：[工作队列](./work_queue.md)

**下一篇**：[完](./client.md)
