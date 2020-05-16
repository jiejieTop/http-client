/*
 * @Author: jiejie
 * @Github: https://github.com/jiejieTop
 * @Date: 2020-05-16 22:23:05
 * @LastEditTime: 2020-05-16 22:58:45
 * @Description: the code belongs to jiejie, please keep the author information and source code according to the license.
 */ 
#include <stdio.h>
#include <unistd.h>
#include <httpclient.h>

extern const char *ca_get();

#define URL1 "http://www.jiedev.com"
#define URL2 "http://httpbin.org/get"       // tcp
#define URL3 "https://httpbin.org/get"       // tls

static int _http_cb(void *e)
{
    http_event_t *event = e;

    printf("\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n\nlen:%ld, data:\n\n"
           "%s\n<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n", event->len, (char*)event->data);
}

void http_get_test(void)
{
    printf("\n---------------------- http_get_test start ----------------------\n");

    http_client_init(ca_get());
    
    http_client_get(URL1, _http_cb);

    http_client_get(URL2, _http_cb);

    http_client_get(URL3, _http_cb); 

    printf("\n---------------------- http_get_test end ----------------------\n");
}

