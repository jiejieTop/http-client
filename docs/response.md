# HTTP应答处理

## 数据结构

```c
typedef struct http_response {
    uint16_t                        status  : 15;   /* response status code */
    uint16_t                        chunked : 1;    /* response status code */
    uint16_t                        length;         /* content length */
    uint16_t                        offset;         /* data offset, skip header */
    http_message_buffer_t           *message;
} http_response_t;
```

应答报文要保存应答的状态码，是否为大块数据，数据的长度，偏移字段（主要是用于跳过头部字段），以及最重要的请求报文数据保存区域，通过**http_message_buffer_t**结构可以支持请求报文动态变化，当内存不足时自动增长内存。


## 外部函数

- 初始化一个响应结构，主要是申请响应报文的内存空间，默认为**HTTP_MESSAGE_BUFFER_GROWTH**。

```c
int http_response_init(http_response_t *rsp)
```







