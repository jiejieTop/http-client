# post 方法测试

主要验证 **http** 的 **post 方法**，此处测试了 2 个 url，有 tcp 直连方式也有 tls 加密方式，均是正常的，**httpbin.org** 这个网站就是用于测试 http 协议的利器。

## 动态图演示

![post](http://qiniu.jiejie01.top/post.gif)

## 代码

```c
#include <stdio.h>
#include <unistd.h>
#include <httpclient.h>

extern const char *ca_get();

#define URL1 "http://httpbin.org/post"      // tcp
#define URL2 "https://httpbin.org/post"     // tls

static int _http_cb(void *e)
{
    http_event_t *event = e;

    printf("%s", (char*)event->data);

    return 0;
}

void http_post_test(void)
{
    printf("\n---------------------- http_post_test start ----------------------\n");

    http_client_init(ca_get());
    
    printf("\n\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n\n");
    http_client_post(URL1, "this is a post test ...", _http_cb);
    printf("\n\n<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n\n");

    printf("\n\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n\n");
    http_client_post(URL2, "this is a post test ...", _http_cb);
    printf("\n\n<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n\n");
    
    printf("\n---------------------- http_post_test end ----------------------\n");
}
```

## 现象

```bash
---------------------- http_post_test start ----------------------


>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

{
  "args": {}, 
  "data": "this is a post test ...\r\n\r", 
  "files": {}, 
  "form": {}, 
  "headers": {
    "Accept": "*/*", 
    "Content-Length": "26", 
    "Content-Type": "text/plain", 
    "Host": "httpbin.org", 
    "User-Agent": "http-client-by-jiejie", 
    "X-Amzn-Trace-Id": "Root=1-5ec2a299-03a9fd77fdaf6ecf2aa5fdc2"
  }, 
  "json": null, 
  "origin": "210.6.9.16", 
  "url": "http://httpbin.org/post"
}


<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<



>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

{
  "args": {}, 
  "data": "this is a post test ...\r\n\r", 
  "files": {}, 
  "form": {}, 
  "headers": {
    "Accept": "*/*", 
    "Content-Length": "26", 
    "Content-Type": "text/plain", 
    "Host": "httpbin.org", 
    "User-Agent": "http-client-by-jiejie", 
    "X-Amzn-Trace-Id": "Root=1-5ec2a29f-b46e156061af6ce0873b3ff0"
  }, 
  "json": null, 
  "origin": "210.6.9.16", 
  "url": "https://httpbin.org/post"
}


<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


---------------------- http_post_test end ----------------------
```

**上一篇**：[get 方法测试](./test_get.md)

**下一篇**：[重定向测试](./test_redirect.md)
