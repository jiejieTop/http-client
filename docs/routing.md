# 路由处理（本地 dns 缓存）

核心功能就是根据 url 中的 host 参数快速得到对应的 IP 地址，避免在连接时进行 dns 解析，耗费时间。

## 数据结构

```c
/**
 * --------------------------------------------------
 * |    hash    |       host        |       ip      |
 * --------------------------------------------------
 * |   0x1234   |  www.jiedev.com   | 192.168.0.122 |
 * |   0x5678   |  www.github.com   | 192.168.0.123 |
 * |   xxxxxx   |  xxxxxxxxxxxxxx   | xxx.xxx.x.xxx |
 * --------------------------------------------------
 */

typedef struct routing {
    uint32_t            hash;
    char                *host;
    char                *ip;
} routing_t;


static routing_t _routing_table[ROUTING_TABLE_SIZE] = {0};
```

维护一个路由表，保存了 **host** 与 **ip** 的关系，路由表的默认大小是 **ROUTING_TABLE_SIZE**，值为 **10**，可由用户修改，路由表的核心就是数组，在实际处理过程中会循环记录，当路由表不足的时候，会选择先缓存的表覆盖掉。

## 外部函数

- host 与 ip 关系的记录。

    ```c
    void routing_record(const char* host, const char *ip);
    ```

- host 与 ip 关系的搜索。

    ```c
    char *routing_search(const char* host);
    ```

## 内部实现

- 哈希算法的实现，其中忽略了大小写字母。

    ```c
    static int _routing_to_lower(int c)

    static uint32_t _routing_hash(const char* str)
    ```

- 匹配算法的实现，依旧是忽略的大小写，它与哈希算法的配合实现了时间复杂度为 **O(n+m)**。

    ```c
    static int _routing_match(const char *str, const char *cmd)
    ```

**上一篇**：[tls 加密传输](./nettype_tls.md)

**下一篇**：[network 网卡](./network.md)
