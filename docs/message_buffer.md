
# HTTP报文处理

## 数据结构

基本的报文描述，可以描述多种报文数据结构，如请求报文的**起始行**、**报文头部**、**报文主体**等，`length`表示该报文被分配的内存空间，`used`表示已使用的内存空间，`data`是内存区域。

```c
typedef struct http_message_buffer {
    size_t              length;     /* number of bytes allocated */
    size_t              used;       /* strlen(data) + 1 */
    char                *data;      /* contents: NUL-terminated string */
} http_message_buffer_t;
```

## 外部函数

- `http_message_buffer_t *http_message_buffer_init(size_t size)` ：初始化报文并分配默认的内存空间`HTTP_MESSAGE_BUFFER_GROWTH`，该宏在`http_defconfig.h`被定义，可由用户修改。

- `void http_message_buffer_concat(http_message_buffer_t *buf, ...)` ：将字符串连接到报文中，可变参数，字符串必须以`NULL`结束，自动处理内存不足的问题，增长的内存大小是`HTTP_MESSAGE_BUFFER_GROWTH`的倍数。

- `void http_message_buffer_append(http_message_buffer_t *buf, const char *str, size_t len)` ：追加指定的`str`字符串数据到报文中，`len`是追加的字符串长度，可自动处理内存不足的问题。


## 内部函数

- `void _http_message_buffer_grow(http_message_buffer_t *buf, size_t newsize)` ：给buf增长内存空间，增长的内存大小是`HTTP_MESSAGE_BUFFER_GROWTH`的倍数。当buf的剩余空间不足以存放指定`newsize`大小的数据时，自动分配内存。


## 依赖

- `platform_memory.c` ：平台层的内存管理
