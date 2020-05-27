# 平台抽象层——socket

- platform_net_socket.c

- platform_net_socket.h

各个平台层的 **socket api** 接口，因为某些平台的 **socket** 并非标准的，因此在此对他们进行了封装，统一了网络的 **socket** 接口，但其内部的操作基本不会相差太大，主要是满足 **socket** 的 **socket、connect、read/recv、write、setsockopt、close** 等操作。

## 外部函数

- 根据指定的服务器地址 **host**、端口号 **port** 与协议类型 **proto** 连接服务器，期间会创建 **socket 描述符**，如果 **host** 是域名将进行域名解析操作，然后 **connect** 服务器，在连接失败的时候 **close** 掉 **socket 描述符**。

    ```c
    int platform_net_socket_connect(const char *host, const char *port, int proto)
    ```

- 读取指定长度 **len** 的数据，默认 **socket** 描述符是阻塞 **io**，当没有数据可读时将持续阻塞，读取到的数据存放到指定的 **buf** 区域，**flags** 一般为 **0**。

    ```c
    int platform_net_socket_recv(int fd, void *buf, size_t len, int flags)
    ```

- 读取指定长度 **len** 的数据，指定阻塞的超时时间 **timeout**，在超时时间到达后，将不再阻塞。

    ```c
    int platform_net_socket_recv_timeout(int fd, unsigned char *buf, int len, int timeout)
    ```

- 向 **socket** 描述符写入指定长度 **len** 的 **buf** 区域的数据，然后发送出去，当不可写的时候会阻塞。

    ```c
    int platform_net_socket_write(int fd, void *buf, size_t len)
    ```

- 向 **socket** 描述符写入指定长度 **len** 的 **buf** 区域的数据，当不可写的时候会阻塞，但在超时时间到达后，将不再阻塞。

    ```c
    int platform_net_socket_write_timeout(int fd, unsigned char *buf, int len, int timeout)
    ```

- 关闭 **socket** 描述符。

    ```c
    int platform_net_socket_close(int fd)
    ```

- 设置 **socket** 描述符为阻塞模式。

    ```c
    int platform_net_socket_set_block(int fd)
    ```

- 设置 **socket** 描述符为非阻塞模式。

    ```c
    int platform_net_socket_set_nonblock(int fd)
    ```

- 设置 **socket** 描述符的其他操作，与 **setsockopt()** 函数的处理是一样的。

    ```c
    int platform_net_socket_setsockopt(int fd, int level, int optname, const void *optval, socklen_t optlen)
    ```

**上一篇**：[平台抽象层—互斥锁](./platform_mutex.md)

**下一篇**：[tcp 直连](./nettype_tcp.md)
