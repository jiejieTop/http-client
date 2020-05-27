# HTTP 应答处理

## HTTP 响应状态码

这些状态码是严格按照 [HTTP协议标准文档——RFC2616](https://tools.ietf.org/html/rfc2616) 编写的，可能很多并未使用上，但为了向后兼容，还是全部写上去了。

```c
typedef enum http_response_status {
    http_response_status_continue                           = 100,      /* informational 1xx  */
    http_response_status_switching_protocols                = 101,
    http_response_status_ok                                 = 200,      /* successful 2xx */
    http_response_status_created                            = 201,
    http_response_status_accepted                           = 202,
    http_response_status_non_authoritative_information      = 203,
    http_response_status_no_content                         = 204,
    http_response_status_reset_content                      = 205,
    http_response_status_partial_content                    = 206,
    http_response_status_multiple_choices                   = 300,      /* redirection 3xx */
    http_response_status_moved_permanently                  = 301,
    http_response_status_found                              = 302,
    http_response_status_see_ther                           = 303,
    http_response_status_not_modified                       = 304,
    http_response_status_use_proxy                          = 305,
    http_response_status_unused                             = 306,      /* unused */
    http_response_status_temporary_redirect                 = 307,
    http_response_status_bad_request                        = 400,      /* client error 4xx */  
    http_response_status_unauthorized                       = 401,
    http_response_status_payment_required                   = 402,      /* this code is reserved for future use */
    http_response_status_forbidden                          = 403,
    http_response_status_not_found                          = 404,
    http_response_status_method_not_allowed                 = 405,
    http_response_status_not_acceptable                     = 406,
    http_response_status_proxy_authentication_required      = 407,
    http_response_status_request_timeout                    = 408,
    http_response_status_conflict                           = 409,
    http_response_status_gone                               = 410,
    http_response_status_length_required                    = 411,
    http_response_status_precondition_failed                = 412,
    http_response_status_request_entity_too_large           = 413,
    http_response_status_request_uri_too_long               = 414,
    http_response_status_unsupported_media_type             = 415,
    http_response_status_requested_range_not_satisfiable    = 416,
    http_response_status_expectation_failed                 = 417,
    http_response_status_internal_server_error              = 500,         /* server error 5xx */
    http_response_status_not_implemented                    = 501,
    http_response_status_bad_gateway                        = 502,
    http_response_status_service_unavailable                = 503,
    http_response_status_gateway_timeout                    = 504,
    http_response_status_http_version_not_supported         = 505,
} http_response_status_t;
```

## 数据结构

```c
typedef struct http_response {
    uint64_t                        length;         /* content length */
    uint32_t                        offset;         /* data offset, skip header */
    http_response_status_t          status;         /* response status code */
    http_message_buffer_t           *message;
} http_response_t;
```

应答报文要保存应答的状态码，是否为大块数据，数据的长度，偏移字段（主要是用于跳过头部字段），以及最重要的请求报文数据保存区域，通过 **http_message_buffer_t** 结构可以支持请求报文动态变化，当内存不足时自动增长内存。

## 外部函数

- 初始化一个响应结构，主要是调用 **http_message_buffer_init()** 函数去申请响应报文的内存空间，默认为 **HTTP_MESSAGE_BUFFER_GROWTH**。

    ```c
    int http_response_init(http_response_t *rsp);
    ```

- 释放一个 http 响应结构，因为在初始化时动态申请了内存，在释放的时候需要使用 **http_message_buffer_release()** 函数去释放内存。

    ```c
    int http_response_release(http_response_t *rsp);
    ```

- **设置** / **获取** 响应结构中的相关信息，如长度、偏移字段、状态等，ps：状态是使用无符号整形数据，但会被转换为 **http_response_status_t** 类型。

    ```c
    uint64_t http_response_get_length(http_response_t *rsp);
    void http_response_set_length(http_response_t *rsp, uint64_t length);

    uint32_t http_response_get_offset(http_response_t *rsp);
    void http_response_set_offset(http_response_t *rsp, uint32_t offset);

    http_response_status_t http_response_get_status(http_response_t *rsp);
    void http_response_set_status(http_response_t *rsp, unsigned int status);
    ```

- 通过响应结构获取相关内容，比如获取报文，报文内部的数据、body、以及长度等，这些封装是为了比较严格的分层，在禁止在外部私自访问响应结构的私有变量。

    ```c
    http_message_buffer_t *http_response_get_message(http_response_t *rsp);
    char *http_response_get_message_data(http_response_t *rsp);
    char *http_response_get_message_body(http_response_t *rsp);
    size_t http_response_get_message_len(http_response_t *rsp);
    ```

**上一篇**：[HTTP 请求](./request.md)

**下一篇**：[拦截器](./interceptor.md)
