# 基础组件功能（utils）

- http_utils.c

- http_utils.h

这两个文件是http中通用的基础组件，可实现对字符串的一系列操作。

## 外部函数

- `size_t http_utils_count_concat(va_list *ap)` ：计算传入的字符串长度，以NULL结尾。

- `void http_utils_concat(char *str, va_list *ap)` ：将字符串连接到指定的`str`后面。

- `char *http_utils_assign_string(char **str, const char *new_str, int len)` ：给`str`填充指定的字符串，在空间不足的情况下自动分配内存空间，`str`可为`NULL`，当`str`有值的时候必须是由动态内存分配的。

- `void http_utils_release_string(char *str)` ：释放字符串的内存空间。

- `int http_utils_atoi(const char *nptr)` ：字符串转`int`类型。

- `char *http_utils_itoa(int value, char *string, int radix)` ：`int`类型转字符串，`string`是保存字符串的空间，`radix`是基数，一般为`10`，转为十进制的数。


## 宏定义

- 健壮性检查：当传入的`item`值为`NULL`或者`false`，则返回指定的`err`，item可以为表达式，如`(aa && bb && cc)`，`err`可以是任意类型的数据，也可为空 **（注意并不是NULL）**。

```c
#define HTTP_ROBUSTNESS_CHECK(item, err) if (!(item)) {                                         \
        HTTP_LOG_E("%s:%d %s()... check for error.", __FILE__, __LINE__, __FUNCTION__);         \
        return err; }
```

## 内部函数

- `int _http_utils_isspace(int x)` ：判断是否为空。

- `int _http_utils_isdigit(int x)` ：判断是否为数字。


