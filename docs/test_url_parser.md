# url 解析测试

通过查看解析的 url，判断解析是否正常。

## 动态图演示

![url_parsing](http://qiniu.jiejie01.top/url_parsing.gif)

## 代码

```c
#include <stdio.h>
#include <unistd.h>
#include <httpclient.h>

#define URL "http://jiejie:test@jiedev.com/index.php?who=jiejie&is=test#frag"

void http_url_parsing_test(void)
{
    printf("\n---------------------- http_url_parsing_test start ----------------------\n");

    http_connect_params_t *connect_params = NULL;
    
    connect_params = http_assign_connect_params();

    http_url_parsing(connect_params, URL);

    printf("url:%s\n", http_get_connect_params_url(connect_params));
    printf("host:%s\n", http_get_connect_params_host(connect_params));
    printf("port:%s\n", http_get_connect_params_port(connect_params));
    printf("scheme:%s\n", http_get_connect_params_scheme(connect_params));
    printf("user:%s\n", http_get_connect_params_user(connect_params));
    printf("password:%s\n", http_get_connect_params_password(connect_params));
    printf("path:%s\n", http_get_connect_params_path(connect_params));
    printf("query:%s\n", http_get_connect_params_query(connect_params));
    printf("farg:%s\n", http_get_connect_params_farg(connect_params));

    http_release_connect_params(connect_params);

    printf("\n---------------------- http_url_parsing_test end ----------------------\n");
    
}
```

## 现象

```bash
---------------------- http_url_parsing_test start ----------------------
url:http://jiejie:test@jiedev.com/index.php?who=jiejie&is=test#frag
host:jiedev.com
port:80
scheme:http
user:jiejie
password:test
path:/index.php
query:who=jiejie&is=test
farg:frag

---------------------- http_url_parsing_test end ----------------------
```

**下一篇**： [get 方法测试](./test_get.md)
