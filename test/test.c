/*
 * @Author: jiejie
 * @Github: https://github.com/jiejieTop
 * @Date: 2019-12-11 21:53:07
 * @LastEditTime: 2020-05-15 23:18:54
 * @Description: the code belongs to jiejie, please keep the author information and source code according to the license.
 */
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <pthread.h>
#include "httpclient.h"
#include <http_interceptor.h>

extern const char *test_ca_get();

#define URL "http://jiejie:test@jiedev.com/index.php?who=jiejie&is=test#frag"
#define URL1 "http://jiejie:test1@jiedev.com/test1/index.php?who=jiejie1&is=test#frag1"
#define URL2 "http://httpbin.org/get"
#define URL3 "http://www.jiedev.com"
#define URL4 "http://httpbin.org/redirect-to?url=http://www.jiedev.com&status_code=301"
#define URL5 "http://httpbin.org/fasa/n/"
#define URL6 "http://httpbin.org/post"

int my_http_cb(void *e)
{
    http_event_t *event = e;
    
    printf("len:%ld, data:\n%s\n", event->len, (char*)event->data);
}

int main(void)
{
    // http_interceptor_t interceptor;
    // http_connect_params_t *connect_params = NULL;

    // memset(&interceptor, 0, sizeof(http_interceptor_t));

    http_log_init();

    // HTTP_LOG_I("url:%s", URL);
    
    // connect_params = http_assign_connect_params();

    // http_url_parsing(connect_params, URL2);
    http_client_init();
    
    http_client_get(URL2, my_http_cb);
    // http_client_post(URL6, "this is a test post...", my_http_cb);

    // HTTP_LOG_I("host:%s", http_get_connect_params_host(connect_params));
    // HTTP_LOG_I("port:%s", http_get_connect_params_port(connect_params));
    // HTTP_LOG_I("scheme:%s", http_get_connect_params_scheme(connect_params));
    // HTTP_LOG_I("user:%s", http_get_connect_params_user(connect_params));
    // HTTP_LOG_I("password:%s", http_get_connect_params_password(connect_params));
    // HTTP_LOG_I("path:%s", http_get_connect_params_path(connect_params));
    // HTTP_LOG_I("query:%s", http_get_connect_params_query(connect_params));
    // HTTP_LOG_I("farg:%s\n", http_get_connect_params_farg(connect_params));

    // http_url_parsing(connect_params, URL1);

    // HTTP_LOG_I("host:%s", http_get_connect_params_host(connect_params));
    // HTTP_LOG_I("port:%s", http_get_connect_params_port(connect_params));
    // HTTP_LOG_I("scheme:%s", http_get_connect_params_scheme(connect_params));
    // HTTP_LOG_I("user:%s", http_get_connect_params_user(connect_params));
    // HTTP_LOG_I("password:%s", http_get_connect_params_password(connect_params));
    // HTTP_LOG_I("path:%s", http_get_connect_params_path(connect_params));
    // HTTP_LOG_I("query:%s", http_get_connect_params_query(connect_params));
    // HTTP_LOG_I("farg:%s\n", http_get_connect_params_farg(connect_params));

    // http_interceptor_init(&interceptor);
    // http_interceptor_set_connect_params(&interceptor, connect_params);
    // http_interceptor_connect(&interceptor);
    // http_interceptor_request(&interceptor, HTTP_REQUEST_METHOD_GET, NULL);

    // http_interceptor_process(&interceptor, connect_params, HTTP_REQUEST_METHOD_GET, NULL, my_http_cb);

    sleep(1);

    return 0;
}