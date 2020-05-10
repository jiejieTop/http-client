# 拦截器

- 拦截器的功能非常重要，可以说是HTTP协议的核心，所有数据都通过它去处理的，可以拦截所有的数据，所以命名为拦截器，它需要实现连接服务器、根据上层的需求去与HTTP服务器进行交互，GET数据、POST数据等，然后还需要处理来自HTTP服务器的响应报文，解析它并且根据响应处理对应的情况，比如重定向、404、重连等，最后获取到数据，递交到上层。

## 状态结构

- 拦截器使用状态机处理，处理过程基本如下：

![interceptor](http://qiniu.jiejie01.top/interceptor.png)

- 状态定义如下：

```c
typedef enum http_interceptor_status {
    http_interceptor_status_invalid = 0x00,
    http_interceptor_status_init,
    http_interceptor_status_connect,
    http_interceptor_status_request,
    http_interceptor_status_response_headers,
    http_interceptor_status_response_body,
    http_interceptor_status_close
} http_interceptor_status_t;
```

## 拦截器数据结构

```c
typedef struct http_interceptor {
    network_t                   *network;
    http_connect_params_t       *connect_params;
    http_request_t              request;
    http_response_t             response;
    http_interceptor_status_t   status;
    http_message_buffer_t       *message;
    uint16_t                    message_len;
    uint16_t                    cmd_timeout;
    struct http_parser          *parser;
    struct http_parser_settings *parser_settings;
    uint16_t                    process_again;
} http_interceptor_t;
```

- network：网卡抽象接口，需要底层网络数据通道打交道，对网络数据的读写操作。

- connect_params：连接参数，用于构造HTTP请求相关的字段。

- request：HTTP请求结构，构造请求相关信息。

- response：HTTP响应结构，用于处理响应的相关信息。

- status：状态，描述拦截器当前的处理状态。

- message：HTTP报文缓冲区，用于存储读写时的报文内容，比如在HTTP请求的时候，它保存了HTTP请求报文的起始行、头部、主体等内容，在HTTP响应的时候，接收响应报文的数据，并将其拷贝到`interceptor->response.message`中保存起来，实现了动态接收 **1 ~ n** 字节的HTTP响应数据。

- message_len：可以由用户指定的报文缓冲区长度，默认值是`HTTP_DEFAULT_BUF_SIZE`。

- cmd_timeout:可以由用户指定的超时时间，默认值是`HTTP_DEFAULT_CMD_TIMEOUT`。

- parser：http解析响应报文的数据结构。

- parser_settings：解析报文时响应的配置，其实是指定相应的回调函数。

- process_again：是否需要重新处理标志。


## 外部函数

- 拦截器初始化，主要实现的功能有：为**网卡结构**分配内存空间、初始化**HTTP请求/响应**相关数据结构、为**HTTP报文message**分配内存空间。

```c
int http_interceptor_init(http_interceptor_t *interceptor)
```

- 拦截器设置连接参数。

```c
int http_interceptor_set_connect_params(http_interceptor_t *interceptor, http_connect_params_t *conn_param)
```

- 拦截器连接服务器，在链接之前要准备好接收响应报文的内容，初始化**interceptor->parser**结构。

```c
int http_interceptor_connect(http_interceptor_t *interceptor)
```

- 发起HTTP请求，指定请求的方法（GET / POST等），如果是POST还要指定报文主体的内容**post_buf**，也可以为NULL。这个函数中会调用 [HTTP请求](./request.md) 相关的函数，构造起始行，构造报文HTTP请求首部、主体等，然后再通过`_http_write_buffer()`函数将数据发送出去。

```c
int http_interceptor_request(http_interceptor_t *interceptor, http_request_method_t mothod, const char *post_buf)
```

- 拦截器状态机处理，过程如前文的状态图所示。

```c
int http_interceptor_process(http_interceptor_t *interceptor,
                             http_connect_params_t *connect_params,
                             http_request_method_t mothod, 
                             const char *post_buf);
```


## 内部函数

与网卡相关的**读/写**数据操作：

- 读数据，支持接收任意字节的数据内容，动态处理内存空间。

```c
static int _http_read_buffer(http_interceptor_t *interceptor, http_message_buffer_t *buf, int length)
```

- 写数据

```c
static int _http_write_buffer(http_interceptor_t *interceptor, unsigned char *buf, int length)
```

- 设置、获取拦截器的状态信息

```c
static void _http_interceptor_set_status(http_interceptor_t *interceptor, http_interceptor_status_t status)

static http_interceptor_status_t _http_interceptor_get_status(http_interceptor_t *interceptor)
```


**上一篇**： [HTTP响应](./response.md)

**下一篇**： [待定](./interceptor.md)

