# 平台抽象层——tcp直连

- platform_nettype_tcp.c

- platform_nettype_tcp.h


## 外部函数

- 读取数据，其实调用的是`platform_net_socket_recv_timeout()`函数。

```c
int platform_nettype_tcp_read(network_t *n, unsigned char *buf, int len, int timeout)
```

- 发送数据，实际调用`platform_net_socket_write_timeout()`函数。

```c
int platform_nettype_tcp_write(network_t *n, unsigned char *buf, int len, int timeout)
```

- 连接服务器，实际调用`platform_net_socket_connect()`函数。

```c
int platform_nettype_tcp_connect(network_t* n)
```

- 终止连接，实际调用`platform_net_socket_close()`函数。

```c
void platform_nettype_tcp_disconnect(network_t* n)
```


**上一篇**：[平台抽象层—socket](./platform_net_socket.md)

**下一篇**： [平台抽象层—tls加密传输](./platform_nettype_tls.md)

