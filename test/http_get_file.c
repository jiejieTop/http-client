/*
 * @Author: jiejie
 * @Github: https://github.com/jiejieTop
 * @Date: 2020-05-16 22:23:05
 * @LastEditTime: 2020-05-16 22:57:54
 * @Description: the code belongs to jiejie, please keep the author information and source code according to the license.
 */ 
#include <stdio.h>
#include <unistd.h>
#include <httpclient.h>

extern const char *ca_get();

#define URL1 "http://httpbin.org/image/png"       // tcp
#define URL2 "https://httpbin.org/image/png"       // tls

static int _http_cb1(void *e)
{
    FILE * fp;
    http_event_t *event = e;

    if (event->len != 0)
        printf("\nreceive http_file.png file!\n");
    
    fp = fopen("http_file.png", "wb");
    fwrite((char*)event->data, 1, event->len, fp);
    fflush(fp);
    fclose(fp);

    return 0;
}

static int _http_cb2(void *e)
{
    FILE * fp;
    http_event_t *event = e;

    if (event->len != 0)
        printf("\nreceive https_file.png file!\n");

    fp = fopen("https_file.png", "wb");
    fwrite((char*)event->data, 1, event->len, fp);
    fflush(fp);
    fclose(fp);

    return 0;
}

void http_get_file_test(void)
{
    printf("\n---------------------- http_get_file_test start ----------------------\n");

    http_client_init(ca_get());
    
    http_client_get(URL1, _http_cb1);

    http_client_get(URL2, _http_cb2);

    printf("\n---------------------- http_get_file_test end ----------------------\n");
}

