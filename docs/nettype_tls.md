# 平台抽象层——tls 加密传输

- nettype_tls.c

- nettype_tls.h

函数的实现略微复杂，主要是封装了加密传输相关的库，初始化相关参数。

## 外部函数

- 读取数据，实调用的是 **ssl** 加密库中读取数据的函数，比如 **mbedtls_ssl_read()** 函数。

    ```c
    int nettype_tls_read(network_t *n, unsigned char *buf, int len, int timeout)
    ```

- 发送数据，实调用的是 **ssl** 加密库中写数据的函数，比如 **mbedtls_ssl_write()** 函数。

    ```c
    int nettype_tls_write(network_t *n, unsigned char *buf, int len, int timeout)
    ```

- 连接服务器，需要为 ssl 分配相关的内存空间，调用 **platform_nettype_tls_init()** 函数初始化相关参数，指定 CA 证书、发起握手、选择加密协议、校验等，然后通过 **ssl** 加密库中写连接的函数 **mbedtls_net_connect()** 与服务器连接。

    ```c
    int nettype_tls_connect(network_t* n)
    ```

- 终止连接，释放相关的资源，实际调用 **mbedtls_ssl_close_notify()** 函数终止连接。

    ```c
    void nettype_tls_disconnect(network_t* n)
    ```

**上一篇**：[tcp直连](./nettype_tcp.md)

**下一篇**：[路由选择](./routing.md)
