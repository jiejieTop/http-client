/*
 * @Author: jiejie
 * @Github: https://github.com/jiejieTop
 * @Date: 2020-05-16 22:22:53
 * @LastEditTime: 2020-05-25 23:37:07
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

    printf("%s", (char*)event->data);

    return 0;
}

void http_post_test(void)
{
    printf("\n---------------------- http_post_test start ----------------------\n");
    
    printf("\n\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n\n");
    http_client_post(URL1, "this is a post test ...", _http_cb);
    printf("\n\n<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n\n");

    printf("\n\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n\n");
    http_client_post(URL2, "this is a post test ...", _http_cb);
    printf("\n\n<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n\n");
    
    printf("\n---------------------- http_post_test end ----------------------\n");
}

