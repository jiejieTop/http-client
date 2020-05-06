# HTTP请求处理

## 数据结构

### 请求方法

定义一个请求方法的映射数组，通过`http_request_method_t`可索引。

```c
typedef enum http_request_method {
    HTTP_REQUEST_METHOD_INVALID      =   0x00,
    HTTP_REQUEST_METHOD_GET          =   0x01,       /* http GET method, http/1.0 */
    HTTP_REQUEST_METHOD_POST         =   0x02,       /* http POST method, http/1.0 */
    HTTP_REQUEST_METHOD_HEAD         =   0x03,       /* http HEAD method, http/1.0 */
    HTTP_REQUEST_METHOD_PUT          =   0x04,       /* http PUT method, http/1.1 */
    HTTP_REQUEST_METHOD_DELETE       =   0x05,       /* http DELETE method, http/1.1 */
    HTTP_REQUEST_METHOD_OPTIONS      =   0x06,       /* http OPTIONS method, http/1.1 */
    HTTP_REQUEST_METHOD_TRACE        =   0x07,       /* http TRACE method, http/1.1 */
    HTTP_REQUEST_METHOD_CONNECT      =   0x08,       /* http CONNECT method, http/1.1, reserved */
    HTTP_REQUEST_METHOD_PATCH        =   0x09        /* http PATCH method, echo test */
} http_request_method_t;
```

```c
static const char *HTTP_METHOD_MAPPING[] = {"INVALID ", "GET ", "POST ", "HEAD ", "PUT ", "DELETE ", "OPTIONS ", "TRACE ", "CONNECT ", "PATCH " };
```

### 请求报文头部

参考[HTTP RFC](https://tools.ietf.org/html/rfc2616#section-4.2)

```
typedef enum http_request_header {
    HTTP_REQUEST_HEADER_INVALID = 0x00,
    HTTP_REQUEST_HEADER_CACHE_CONTROL,
    HTTP_REQUEST_HEADER_CONNECTION,
    HTTP_REQUEST_HEADER_DATE,
    HTTP_REQUEST_HEADER_PRAGMA,
    HTTP_REQUEST_HEADER_TRAILER,
    HTTP_REQUEST_HEADER_TRANSFER_ENCODING,
    HTTP_REQUEST_HEADER_UPGRADE,
    HTTP_REQUEST_HEADER_VIA,
    HTTP_REQUEST_HEADER_WARNING,
    HTTP_REQUEST_HEADER_ACCEPT,
    HTTP_REQUEST_HEADER_ACCEPT_CHARSET,
    HTTP_REQUEST_HEADER_ACCEPT_ENCODING,
    HTTP_REQUEST_HEADER_ACCEPT_LANGUAGE,
    HTTP_REQUEST_HEADER_AUTHORIZATION,
    HTTP_REQUEST_HEADER_EXPECT,
    HTTP_REQUEST_HEADER_FROM,
    HTTP_REQUEST_HEADER_HOST,
    HTTP_REQUEST_HEADER_IF_MATCH,
    HTTP_REQUEST_HEADER_IF_MODIFIED_SINCE,
    HTTP_REQUEST_HEADER_IF_NONE_MATCH,
    HTTP_REQUEST_HEADER_IF_RANGE,
    HTTP_REQUEST_HEADER_IF_UNMODIFIED_SINCE,
    HTTP_REQUEST_HEADER_MAX_FORWARDS,
    HTTP_REQUEST_HEADER_PROXY_AUTHORIZATION,
    HTTP_REQUEST_HEADER_RANGE,
    HTTP_REQUEST_HEADER_REFERER,
    HTTP_REQUEST_HEADER_TE,
    HTTP_REQUEST_HEADER_USER_AGENT
} http_request_header_t;
```

头部映射数组：

```c
static const char *HTTP_REQUEST_HEADERS_MAPPING[] = {
    "INVALID",
    "Cache-Control: ",
    "Connection: ",
    "Date: ",
    "Pragma: ",
    "Trailer: ",
    "Transfer-Encoding: ",
    "Upgrade: ",
    "Via: ",
    "Warning: ",
    "Accept: ",
    "Accept-Charset: ",
    "Accept-Encoding: ",
    "Accept-Language: ",
    "Authorization: ",
    "Expect: ",
    "From: ",
    "Host: ",
    "If-Match: ",
    "If-Modified-Since: ",
    "If-None-Match: ",
    "If-Range: ",
    "If-Unmodified-Since: ",
    "Max-Forwards: ",
    "Proxy-Authorization: ",
    "Range: ",
    "Referer: ",
    "TE: ",
    "User-Agent: "
};
```

### 请求报文

包括起始行、头部、主体，均采用[http_message_buffer_t](./message_buffer.md)描述。

```c
typedef struct http_request_message {
    http_message_buffer_t           *line;  
    http_message_buffer_t           *header;
    http_message_buffer_t           *body;
} http_request_message_t;
```

### 请求

包括请求方法、请求报文、标志等。

```c
typedef struct http_request {
    http_request_method_t           http_method;
    http_request_message_t          req_msg;
    union {
        uint32_t                    flag;
        struct {
            uint32_t no_keep_alive  : 8;
            uint32_t no_is_http11   : 8;
            uint32_t othre          : 16;
        } flag_t;
    } req_flag;
} http_request_t;
```

## 外部函数

- `int http_request_init(http_request_t *req)` ：http请求初始化，主要是为请求报文分配内存空间。

- `int http_request_start_line(http_request_t *req,  http_request_method_t method, const char *path)` ：构造请求起始行，传入请求的方法、路径。

- `void http_request_add_header(http_request_t *req, const char *key, const char *value)` ：添加请求头部的内容，指定`key： value`，`key`字段内容可参考[HTTP RFC](https://tools.ietf.org/html/rfc2616#section-4.2)，也可参考`HTTP_REQUEST_HEADERS_MAPPING`数组的内容。

- `void http_request_add_header_form_index(http_request_t *req, http_request_header_t header, const char *value)` ：通过索引的方式添加请求头部的`value`字段。





