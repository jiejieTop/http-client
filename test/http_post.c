/*
 * @Author: jiejie
 * @Github: https://github.com/jiejieTop
 * @Date: 2020-05-16 22:22:53
 * @LastEditTime: 2020-05-16 22:57:01
 * @Description: the code belongs to jiejie, please keep the author information and source code according to the license.
 */ 

#include <stdio.h>
#include <unistd.h>
#include <httpclient.h>

extern const char *ca_get();

#define URL1 "http://httpbin.org/post"      // tcp
#define URL2 "https://httpbin.org/post"     // tls

static int _http_cb(void *e)
{
    http_event_t *event = e;

    printf("\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n\nlen:%ld, data:\n\n"
           "%s\n<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n", event->len, (char*)event->data);
}

void http_post_test(void)
{
    printf("\n---------------------- http_post_test start ----------------------\n");

    http_client_init(ca_get());
    
    http_client_post(URL1, "this is a post test ...", _http_cb);

    http_client_post(URL2, "this is a post test ...", _http_cb);
    
    printf("\n---------------------- http_post_test end ----------------------\n");
}

