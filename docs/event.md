# HTTP回调事件

抽象了 HTTP 处理的回调事件，让多个组件都可以使用它进行回调的处理。

## 事件的类型

主要是针对 HTTP 处理过程，比如在发起连接的时候、请求的时候、响应的时候、在解析到响应字段头部的时候、解析到响应字段主体的时候、上报数据的时候、释放的时候等，它们的值是独立占用一个 bit 的，方便在上层做判断。

```c
typedef enum http_event_type {
    http_event_type_on_error = ~0x0,
    http_event_type_on_connect = 0x0001 << 0,
    http_event_type_on_request = 0x0001 << 1,
    http_event_type_on_response = 0x0001 << 2,
    http_event_type_on_headers = 0x0001 << 3,
    http_event_type_on_body = 0x0001 << 4,
    http_event_type_on_submit = 0x0001 << 5,
    http_event_type_on_release = 0x0001 << 6,
    http_event_type_all = 0x0001 << 7
} http_event_type_t;
```

## 事件的回调函数类型

```c
typedef int (*http_event_cb_t)(void *e);
```

## 数据结构

包含了 **事件的类型**、**当前上下文的结构体指针 context**、**数据内容及长度**、**回调的处理函数**。

```c
typedef struct http_event {
    http_event_type_t       type;           /* http_event_type_t type */
    void                    *context;
    void                    *data;
    size_t                  len;
    http_event_cb_t         cb;
} http_event_t;
```

## 外部函数

- 事件的初始化，需要动态申请内存。

    ```c
    http_event_t *http_event_init(void);
    ```

- 事件释放，释放申请的内存空间。

    ```c
    void http_event_release(http_event_t *event);
    ```

- 注册事件的回调函数，在事件发生的时候通过这个函数告知上层。

    ```c
    int http_event_register(http_event_t *event, http_event_cb_t cb);
    ```

- 触发事件回调函数，传入的参数有 **事件结构**、**类型**、**当前上下文的结构体指针 context**、**数据内容及长度**。

    ```c
    int http_event_dispatch(http_event_t *event, http_event_type_t type, void *context, void *data, size_t len);
    ```

**上一篇**：[通用组件功能](./general.md)

**下一篇**：[url 解析器](./url_parser.md)
