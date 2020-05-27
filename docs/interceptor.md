# 拦截器

拦截器的功能非常重要，可以说是 **HTTP** 协议的核心，所有数据都通过它去处理的，可以拦截所有的数据，所以命名为 **拦截器**，它需要实现连接服务器、根据上层的需求去与 **HTTP** 服务器进行交互，**GET** 数据、**POST** 数据等，然后还需要处理来自 **HTTP** 服务器的响应报文，解析它并且根据响应处理对应的情况，比如 **重定向、404、重连、认证** 等，最后获取到数据，递交到上层。

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
    char                        *buffer;
    size_t                      buffer_len;
    size_t                      cmd_timeout;
    size_t                      data_process;
    struct http_parser          *parser;
    struct http_parser_settings *parser_settings;
    http_event_t                *evetn;
    void                        *owner;
    union {
        uint32_t                all_flag;
        struct {
            uint32_t again              : 4;
            uint32_t redirects          : 4;
            uint32_t retry              : 4;
            uint32_t authenticate       : 4;
            uint32_t chunked            : 4;
            uint32_t chunked_complete   : 4;
            uint32_t complete           : 4;
            uint32_t keep_alive         : 4;
        } flag_t;
    } flag;
} http_interceptor_t;
```

- network：网卡抽象接口，需要底层网络数据通道打交道，对网络数据的读写操作。

- connect_params：连接参数，用于构造 **HTTP 请求** 相关的字段。

- request：**HTTP 请求** 结构，构造请求相关信息。

- response：**HTTP 响应** 结构，用于处理响应的相关信息。

- status：**拦截器的状态**，描述拦截器当前的处理状态。

- message：**HTTP 报文缓冲区**，用于存储写数据时的报文内容，比如在 **HTTP 请求** 的时候，它保存了 **HTTP 请求** 报文的起始行、头部、主体等内容。

- buffer：**接收数据缓冲区**，接收完数据就递交到上层去。

- buffer_len：可以由用户指定的报文缓冲区长度，默认值是 **HTTP_DEFAULT_BUF_SIZE**。

- data_process：已经处理的数据长度。

- cmd_timeout: 可以由用户指定的超时时间，默认值是 **HTTP_DEFAULT_CMD_TIMEOUT**。

- parser：**http 解析响应报文** 的数据结构。

- parser_settings：解析报文时响应的配置，其实是指定相应的回调函数。

- evetn：事件回调，在需要的时候通过回调函数处理，将数据上报，比如 **发生错误的时候、在请求之前、在接收到应答的时候、在解析报文的时候、在解析完成报文的时候、上报 body 数据的时候** 等等。

- owner：**所有者属性**，该拦截器是属于哪个上层结构（**client**）的。

- flag：使用了共用体与结构体描述的一个拦截器标志位：

  - again：是否需要重新处理。
  
  - redirects：重定向标志位，拦截器自动处理重定向的内容，这个操作对用户来说是透明的。
  
  - retry：重新尝试。保留未用。
  
  - authenticate：需要认证。

  - chunked：接收的数据是分块的。
  
  - chunked_complete：所有分块数据接收完毕。
  
  - complete：数据接收完成。
  
  - keep_alive：指定长连接。

## 外部函数

- 拦截器初始化，主要实现的功能有：为 **网卡结构** 分配内存空间、初始化 **HTTP 请求/响应** 相关数据结构、初始化 **HTTP 回调事件 evetn**、为 **HTTP 报文 message** 分配内存空间、为 **HTTP 解析器 parser、parser_settings** 分配相关的内存空间并且初始它们。

    ```c
    int http_interceptor_init(http_interceptor_t *interceptor)
    ```

- 拦截器的释放操作，因为在处理 HTTP 的时候会动态分配非常多的内存空间，在最后应该要回收它们，注意，此函数不仅回收了 **http_interceptor_init()** 函数分配的内存空间，还会回收在 **处理 HTTP 请求、应答、解析** 时候动态申请的内存空间。

    ```c
    int http_interceptor_release(http_interceptor_t *interceptor);
    ```

- 设置 ca 证书

```c
void http_interceptor_set_ca(const char *ca);
```

- 设置拦截器的所有者属性，指定拦截器归属谁所有，可以为 NULL，但一般不为 NULL。

    ```c
    void http_interceptor_set_owner(http_interceptor_t *interceptor, void *owner);
    ```

- 注册回调函数事件，在合适的时候会通过回调函数告知上层，一般来说在回调函数中需要判断是何种事件产生的回调，决定是否需要处理，回调函数的类型是 **http_event_cb_t**，详细内容查看 [HTTP回调事件](./event.md)。

    ```c
    void http_interceptor_event_register(http_interceptor_t *interceptor, http_event_cb_t cb);
    ```

- 拦截器设置连接参数。

    ```c
    int http_interceptor_set_connect_params(http_interceptor_t *interceptor, http_connect_params_t *conn_param)
    ```

- 拦截器连接服务器，在链接之前要准备好接收响应报文的内容，初始化 **interceptor->parser** 结构。

    ```c
    int http_interceptor_connect(http_interceptor_t *interceptor)
    ```

- 发起 **HTTP 请求**，指定请求的方法（GET / POST 等），如果是 POST 还要指定报文主体的内容 **post_buf**，也可以为 **NULL**。这个函数中会调用 [HTTP请求](./request.md) 相关的函数，构造起始行，构造报文 **HTTP 请求** 首部、主体等，然后再通过 **_http_write_buffer()** 函数将数据发送出去。

    ```c
    int http_interceptor_request(http_interceptor_t *interceptor, http_request_method_t mothod, const char *post_buf)
    ```

- 拦截器状态机处理，过程如前文的状态图所示。

    ```c
    int http_interceptor_process(http_interceptor_t *interceptor,
                                http_connect_params_t *connect_params,
                                http_request_method_t mothod, 
                                void *post_buf,
                                void *owner,
                                http_event_cb_t cb);
    ```

## 内部函数

与网卡相关的 **读/写** 数据操作：

- 读数据。

    ```c
    static int _http_read_buffer(http_interceptor_t *interceptor, size_t length)
    ```

- 写数据。

    ```c
    static int _http_write_buffer(http_interceptor_t *interceptor, unsigned char *buf, int length)
    ```

- 设置、获取拦截器的状态信息

    ```c
    static void _http_interceptor_set_status(http_interceptor_t *interceptor, http_interceptor_status_t status)

    static http_interceptor_status_t _http_interceptor_get_status(http_interceptor_t *interceptor)
    ```

- 设置解析器的回调函数，注意这些是内部的处理函数。

    ```c
    static int _http_interceptor_parser_setting(http_interceptor_t *interceptor)
    {
        interceptor->parser_settings->on_url = _http_on_url;
        interceptor->parser_settings->on_status = _http_on_status;
        interceptor->parser_settings->on_header_field = _http_on_header_field;
        interceptor->parser_settings->on_header_value = _http_on_header_value;
        interceptor->parser_settings->on_headers_complete = _http_on_headers_complete;
        interceptor->parser_settings->on_message_begin = _http_on_message_begin;
        interceptor->parser_settings->on_message_complete = _http_on_message_complete;
        interceptor->parser_settings->on_body = _http_on_body;
        interceptor->parser_settings->on_chunk_header = _http_on_chunk_header;
        interceptor->parser_settings->on_chunk_complete = _http_on_chunk_complete;
        interceptor->parser->data = interceptor;
    }
    ```

**上一篇**：[HTTP 响应](./response.md)

**下一篇**：[工作队列](./work_queue.md)
