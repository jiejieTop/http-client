# url解析器

- http_url_parser.c

- http_url_parser.h

解析 url，产生相应的参数。

## 数据结构

**url** 的各个字段内容：

```c
typedef struct http_connect_params {
    char                        *http_url;
    char                        *http_scheme;
    char                        *http_host;
    char                        *http_user;
    char                        *http_password;
    char                        *http_path;
    char                        *http_query;
    char                        *http_farg;
    char                        *http_cert_pem;
    uint16_t                    http_port;
} http_connect_params_t;
```

## 外部函数

- 解析 **url** 的各个字段内容并且保存在 **http_connect_params_t** 结构中。

```c
int http_url_parsing(http_connect_params_t *connect_params, const char *url)
```

## 内部函数

## 依赖

- http_parser.c ：http_parser_parse_url()

- [http_utils.c](./utils.md) ：http_utils_assign_string()

**上一篇**：[HTTP 回调事件](./event.md)

**下一篇**：[HTTP 报文处理](./message_buffer.md)
