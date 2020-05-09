# 网卡抽象接口

- network.c

- network.h

这两个文件是对网卡进行抽象，提供上层的read、write、connect、disconnect等操作，对下选择数据通道，可以是tls加密传输，也可以是tcp直连。


## 数据结构

```c
#ifdef HTTP_NETWORK_TYPE_TLS
typedef struct network_ssl_params {
    const char		            *ca_crt;
    size_t 		                ca_crt_len;
#if defined(MBEDTLS_FS_IO)
    const char                  *cert_file;            // public certificate file
    const char                  *key_file;             // pravite certificate file
#else
#if defined(MBEDTLS_KEY_EXCHANGE__SOME__PSK_ENABLED)
    const char                  *psk;                  // PSK string
    const char                  *psk_id;               // PSK ID
    size_t                      psk_length;            // PSK length
#endif
#endif
    unsigned int                timeout_ms;            // SSL handshake timeout in millisecond
} network_ssl_params_t;
#endif /* HTTP_NETWORK_TYPE_TLS */


typedef struct network_params {
    char                        *addr;
    char                        *port;
#ifdef HTTP_NETWORK_TYPE_TLS
    network_ssl_params_t        network_ssl_params;
    void                        *nettype_tls_params;
#endif /* HTTP_NETWORK_TYPE_TLS */
} network_params_t;


typedef struct network {
    int                     socket;
    network_params_t        network_params;
    int                     (*connect)(struct network *);
    void                    (*disconnect)(struct network *);
    int                     (*read)(struct network *, unsigned char *, int, int);
    int                     (*write)(struct network *, unsigned char *, int, int);
} network_t;
```

- 封装了`ssl`相关的参数`network_ssl_params_t`，目前暂时只有`mbedtls`的，`openssl`在后续完善实现。

- 封装了网卡的参数`network_params_t`，比如要连接的`addr`、`port`等。

- 网卡`network_t`设备，记录`socket`描述符，提供`connect、disconnect、read、write`等操作。

## 外部函数

- 网卡初始化，设置相关参数。

```c
int network_init(network_t* n, network_params_t* network_params)
```

- 从底层读数据，指定数据存储区域`buf`、数据长度`len`、读超时的时间`timeout`。

- network设置目标主机地址与端口号。

```c
int network_set_addr_port(network_t* n, char *addr, char *port)
```

```c
int network_read(network_t* n, unsigned char* buf, int len, int timeout)
```

- 向底层写数据，指定数据的地址`buf`、数据长度`len`、写超时的时间`timeout`。

```c
int network_write(network_t* n, unsigned char* buf, int len, int timeout)
```

- 与服务器建立连接，如果是`tcp`直连，将会申请`socket`描述符，向服务器发起连接请求、三次握手后建立连接；如果是`tls`加密传输，将发生`ssl`握手操作。

```c
int network_connect(network_t* n)
```

- 释放网卡，**close** `socket`描述符。

```c
void network_release(network_t* n)
```

**注意**：以上的这些函数都会根据配置指定的数据通道选择不同的传输方式，当使用加密传输则通过tls通道，否则就使用tcp直连的方式，可以参考框架图。


**上一篇**：[平台抽象层—tls加密传输](./platform_nettype_tls.md)

**下一篇**： [基础组件功能](./utils.md)


