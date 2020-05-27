# 网卡抽象接口

- network.c

- network.h

这两个文件是对网卡进行抽象，提供上层的 **read、write、connect、disconnect** 等操作，对下选择数据通道，可以是 **tls** 加密传输，也可以是 **tcp** 直连。

## 数据结构

```c
typedef struct network {
    int                         socket;
    int                         channel;        /* tcp or tls */
    const char                  *host;
    const char                  *port;
    const char		            *ca_crt;
    size_t 		                ca_crt_len;
    unsigned int                timeout_ms;     /* SSL handshake timeout in millisecond */
    void                        *nettype_tls_params;
} network_t;
```

- 封装了 **ssl** 相关的参数 **nettype_tls_params**，目前暂时只有 **mbedtls** 的，**openssl** 在后续完善实现。

- 自动选择数据通道 **channel**，比如 tcp 直连或者 tls 加密。

- 封装了网卡的参数，比如要连接的 **host**、**port** 等。

- 记录 **socket** 描述符，向上层提供 **connect、disconnect、read、write** 等操作。

- 还有 ca 证书、长度、tls 握手时间等 **ca_crt**、 **ca_crt_len** 、**timeout_ms**。

## 外部函数

- 网卡初始化，设置相关参数，**IP 地址**，**端口号**，如果是 **tls** 数据通道的话还要指定 **ca 证书**，如果指定了 **ca 证书**，这里它会 **自动选择 tls 数据通道** 的。

    ```c
    int network_init(network_t *n, const char *host, const char *port, const char *ca);
    ```

- 设置 ca 证书。

    ```c
    int network_set_ca(network_t *n, const char *ca);
    ```

- 设置数据通道。

    ```c
    int network_set_channel(network_t *n, int channel);
    ```

- network 设置目标主机地址与端口号。

    ```c
    int network_set_host_port(network_t* n, char *host, char *port)
    ```

- 从底层读数据，指定数据存储区域 **buf**、数据长度 **len**、读超时的时间 **timeout**。

    ```c
    int network_read(network_t* n, unsigned char* buf, int len, int timeout)
    ```

- 向底层写数据，指定数据的地址 **buf**、数据长度 **len**、写超时的时间 **timeout**。

    ```c
    int network_write(network_t* n, unsigned char* buf, int len, int timeout)
    ```

- 与服务器建立连接，如果是 **tcp** 直连，将会申请 **socket** 描述符，向服务器发起连接请求、三次握手后建立连接；如果是 **tls** 加密传输，将发生 **ssl** 握手操作。

    ```c
    int network_connect(network_t* n)
    ```

- 释放网卡，**close socket** 描述符。

    ```c
    void network_release(network_t* n)
    ```

**注意**：以上的这些函数都会自动选择不同的传输方式，当使用加密传输则通过 **tls 通道**，否则就使用 **tcp 直连** 的方式，可以参考框架图。

**上一篇**：[路由选择](./routing.md)

**下一篇**：[基础组件功能](./utils.md)
