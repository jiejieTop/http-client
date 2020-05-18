# httpclient

这应该是用户最容易接触到的上层应用了，虽然用户可以接触它，但是我并不打算将client对外公开操作，除非很熟悉（至少我目前是这样子打算的），因此我将它设置成内部管理。

## 数据结构

```c
typedef struct http_client {
    http_connect_status_t               status;
    http_list_t                         list;
    platform_mutex_t                    global_lock;
    http_interceptor_t                  *interceptor;
    http_connect_params_t               *connect_params;
    uint32_t                            interest_event;
    http_event_t                        *event;
} http_client_t;
```

- list : **http-client**内部维护了两条链表，分别是**已使用**与**空闲**链表，而list则是用于挂载到这两个列表中的：

```c
static http_list_t _http_client_free_list;
static http_list_t _http_client_used_list;
```

- interceptor ：拥有的拦截器。

- connect_params ：指向连接参数结构。

- interest_event ：感兴趣的事件，在拦截器收到数据的时候触发回调函数，而通过感兴趣的事件选择是否上报到用户空间。

- event ：事件回调处理。

## 外部函数

- **http-client**的初始化，应该称之为系统初始化更合适，它会初始化内部管理的**client pool**，用链表的形式去管理所有的空闲 **client**，数量由用户指定**HTTP_CLIENT_POOL_SIZE**，默认为5。

```c
int http_client_init(const char *ca);
```

- 分配**client**，其实就是从空闲链表**_http_client_free_list**中取出一个**client**。

```c
http_client_t *http_client_assign(void);
```

- **http-client**释放操作，这应该叫将**client**放回空闲链表**_http_client_free_list**中。

```c
void http_client_release(http_client_t *c);
```

- 用户接口，**http get**数据，通过回调函数上报数据，它只需**url参数**与对应的回调函数即可，在内部会从空闲链表**_http_client_free_list**中分配一个**client**用于处理url，处理完了会释放**client**。

```c
int http_client_get(const char *url, http_event_cb_t cb);
```

- 用户接口，**http post**数据，指定要**post**的**url**与数据**内容**。

```c
int http_client_post(const char *url, void *data ,http_event_cb_t cb);
```

- 注册感兴趣的事件，在产生该事件的时候将会告知应用层。

```c
void http_client_set_interest_event(http_client_t *c, http_event_type_t event);
```

## 内部函数

- **client pool**的初始化，其实就是根据指定的 **client** 数量去初始化 **client** 池，并挂载到空闲链表**_http_client_free_list**中，在需要的时候从该链表取下。

```c
void *_http_client_pool_init(void)
```

- client的初始化，主要就是分配相关的内存空间。

```c
int _http_client_internal_init(void)
```

- 根据用户指定的感兴趣的事件进行回调处理。

```c
static int _http_client_internal_event_handle(void *e)
```

**上一篇**：[拦截器](./interceptor.md)

**下一篇**： [待定](./client.md)

