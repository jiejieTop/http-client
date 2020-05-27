# 基础组件功能（utils）

- http_utils.c

- http_utils.h

这两个文件是 **http** 中通用的基础组件，可实现对字符串的一系列操作。

## 外部函数

- 计算传入的字符串长度，以 **NULL** 结尾。

    ```c
    size_t http_utils_count_concat(va_list *ap)
    ```

- 将字符串连接到指定的 **str** 后面。

    ```c
    void http_utils_concat(char *str, va_list *ap)
    ```

- 给 **str** 填充指定的字符串，在空间不足的情况下自动分配内存空间，**str** 可为 **NULL**，当 **str** 有值的时候必须是由动态内存分配的。

    ```c
    char *http_utils_assign_string(char **str, const char *new_str, int len)
    ```

- 释放字符串的内存空间。

    ```c
    void http_utils_release_string(char *str)
    ```

- 字符串转 **int** 类型。

    ```c
    int http_utils_atoi(const char *nptr)
    ```

- **int** 类型转字符串，**string** 是保存字符串的空间，**radix** 是基数，一般为 **10**，转为十进制的数。

    ```c
    char *http_utils_itoa(int value, char *string, int radix)
    ```

- 匹配字符串操作，指定匹配的字符串长度、支持忽略大小写等。

    ```c
    int http_utils_match(const char *str1, const char *str2)
    int http_utils_nmatch(const char *str1, const char *str2, int n)
    int http_utils_ignore_case_match(const char *str1, const char *str2)
    int http_utils_ignore_case_nmatch(const char *str1, const char *str2, int n)
    ```

## 宏定义

- 健壮性检查：当传入的 **item** 值为 **NULL** 或者 **false**，则返回指定的 **err**，item 可以为表达式，如 `(aa && bb && cc)`，**err** 可以是任意类型的数据，也可为空 **（注意并不是 NULL）**。

    ```c
    #define HTTP_ROBUSTNESS_CHECK(item, err) if (!(item)) {                                         \
            HTTP_LOG_E("%s:%d %s()... check for error.", __FILE__, __LINE__, __FUNCTION__);         \
            return err; }
    ```

## 内部函数

- 判断是否为空。

    ```c
    int _http_utils_isspace(int x)
    ```

- 判断是否为数字。

    ```c
    int _http_utils_isdigit(int x) 
    ```

**上一篇**：[network 网卡](./network.md)

**下一篇**：[通用组件功能](./general.md)
