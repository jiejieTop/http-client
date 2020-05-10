# HTTP通用组件

## 数据结构

- 连接参数信息，它实际上是url的各个字段内容，它在url解析器中被使用，解析url产生相关信息保存在这里。

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

## 相关的宏定义

- 常用的http、https协议端口

```c
#define DEFAULT_HTTP_PORT   "80"
#define DEFAULT_HTTPS_PORT  "443"
```

- 让编译器预处理，产一系列功能操作相同的函数，**获取/设置**连接参数的相关信息，如url、scheme、host、user、password、path、query等。

```c
#define HTTP_GET_CONNECT_PARAMS_DEFINE(name, type, res)                                     \
    type http_get_connect_params_##name(http_connect_params_t *connect_params) {            \
        HTTP_ROBUSTNESS_CHECK((connect_params), res);                                       \
        return connect_params->http_##name;                                                 \
    }

#define HTTP_SET_CONNECT_PARAMS_DEFINE(name, type, res)                                     \
    type http_set_connect_params_##name(http_connect_params_t *connect_params, type t) {    \
        HTTP_ROBUSTNESS_CHECK((connect_params && t), res);                                  \
        connect_params->http_##name = t;                                                    \
        return connect_params->http_##name;                                                 \
    }

#define HTTP_SET_AND_GET_CONNECT_PARAMS_DEFINE(name, type, res)         \
    HTTP_SET_CONNECT_PARAMS_DEFINE(name, type, res)                     \
    HTTP_GET_CONNECT_PARAMS_DEFINE(name, type, res)

#define HTTP_SET_AND_GET_CONNECT_PARAMS_STATEMENT(name, type)           \
    HTTP_SET_CONNECT_PARAMS_STATEMENT(name, type)                       \
    HTTP_GET_CONNECT_PARAMS_STATEMENT(name, type)
```

```c
HTTP_SET_AND_GET_CONNECT_PARAMS_DEFINE(url, char*, NULL)
HTTP_SET_AND_GET_CONNECT_PARAMS_DEFINE(scheme, char*, NULL)
HTTP_SET_AND_GET_CONNECT_PARAMS_DEFINE(host, char*, NULL)
HTTP_SET_AND_GET_CONNECT_PARAMS_DEFINE(user, char*, NULL)
HTTP_SET_AND_GET_CONNECT_PARAMS_DEFINE(password, char*, NULL)
HTTP_SET_AND_GET_CONNECT_PARAMS_DEFINE(path, char*, NULL)
HTTP_SET_AND_GET_CONNECT_PARAMS_DEFINE(query, char*, NULL)
HTTP_SET_AND_GET_CONNECT_PARAMS_DEFINE(farg, char*, NULL)
HTTP_SET_AND_GET_CONNECT_PARAMS_DEFINE(port, char*, NULL)
```

## 外部函数

- 在编译器的预处理后会产生以下函数与声明，函数看名字就知道是怎么用的了，非常简单，通过宏定义只是减少了书写的代码量而已。

```c
http_get_connect_params_url(http_connect_params_t *connect_params)
http_get_connect_params_scheme(http_connect_params_t *connect_params)
http_get_connect_params_host(http_connect_params_t *connect_params)
http_get_connect_params_port(http_connect_params_t *connect_params)
http_get_connect_params_user(http_connect_params_t *connect_params)
http_get_connect_params_password(http_connect_params_t *connect_params)
http_get_connect_params_path(http_connect_params_t *connect_params)
http_get_connect_params_query(http_connect_params_t *connect_params)
http_get_connect_params_farg(http_connect_params_t *connect_params)

http_set_connect_params_url(http_connect_params_t *connect_params, char* t)
http_set_connect_params_scheme(http_connect_params_t *connect_params, char* t)
http_set_connect_params_host(http_connect_params_t *connect_params, char* t)
http_set_connect_params_port(http_connect_params_t *connect_params, char* t)
http_set_connect_params_user(http_connect_params_t *connect_params, char* t)
http_set_connect_params_password(http_connect_params_t *connect_params, char* t)
http_set_connect_params_path(http_connect_params_t *connect_params, char* t)
http_set_connect_params_query(http_connect_params_t *connect_params, char* t)
http_set_connect_params_farg(http_connect_params_t *connect_params, char* t)
```

- 为连接参数结构分配内存与释放内存

```c
http_connect_params_t *http_assign_connect_params(void)

void http_release_connect_params(http_connect_params_t *connect_params)
```


**上一篇**：[基础组件功能](./utils.md)

**下一篇**： [url解析器](./url_parser.md)

