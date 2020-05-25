/*
 * @Author: jiejie
 * @Github: https://github.com/jiejieTop
 * @Date: 2020-05-16 22:40:21
 * @LastEditTime: 2020-05-25 22:43:00
 * @Description: the code belongs to jiejie, please keep the author information and source code according to the license.
 */ 


#define URL4 

#include <stdio.h>
#include <unistd.h>
#include <httpclient.h>

extern const char *ca_get();

#define URL1 "http://httpbin.org/redirect-to?url=http://www.jiedev.com&status_code=301"         // tcp
#define URL2 "https://httpbin.org/redirect-to?url=http://www.jiedev.com&status_code=301"         // tls

static int _http_cb(void *e)
{
    http_event_t *event = e;

    printf("%s", (char*)event->data);

    return 0;
}

void http_redirect_test(void)
{
    printf("\n---------------------- http_redirect_test start ----------------------\n");
    
    printf("\n\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n\n");
    http_client_get(URL1, _http_cb);
    printf("\n\n<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n\n");

    printf("\n\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n\n");
    http_client_get(URL2, _http_cb);
    printf("\n\n<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n\n");

    printf("\n---------------------- http_redirect_test end ----------------------\n");

}
