# tcp 直连

- nettype_tcp.c

- nettype_tcp.h

## 外部函数

- 读取数据，其实调用的是 **platform_net_socket_recv_timeout()** 函数。

    ```c
    int nettype_tcp_read(network_t *n, unsigned char *buf, int len, int timeout)
    ```

- 发送数据，实际调用 **platform_net_socket_write_timeout()** 函数。

    ```c
    int nettype_tcp_write(network_t *n, unsigned char *buf, int len, int timeout)
    ```

- 连接服务器，实际调用 **platform_net_socket_connect()** 函数。

    ```c
    int nettype_tcp_connect(network_t* n)
    ```

- 终止连接，实际调用 **platform_net_socket_close()** 函数。

    ```c
    void nettype_tcp_disconnect(network_t* n)
    ```

**上一篇**：[平台抽象层—socket](./platform_net_socket.md)

**下一篇**：[tls 加密传输](./nettype_tls.md)
