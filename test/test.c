/*
 * @Author: jiejie
 * @Github: https://github.com/jiejieTop
 * @Date: 2019-12-11 21:53:07
 * @LastEditTime: 2020-05-03 22:28:40
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

#define URL "https://jiejie:test@jiedev.com/test/index.php?who=jiejie&is=test#frag"


int main(void)
{
    http_interceptor_t interceptor;

    memset(&interceptor, 0, sizeof(http_interceptor_t));

    http_log_init();

    HTTP_LOG_I("url:%s", URL);
    
    http_interceptor_connect_request(&interceptor, URL, HTTP_REQUEST_METHOD_GET);

    HTTP_LOG_I("host:%s", interceptor.connect_params.http_host);
    HTTP_LOG_I("port:%d", interceptor.connect_params.http_port);
    HTTP_LOG_I("scheme:%s", interceptor.connect_params.http_scheme);
    HTTP_LOG_I("username:%s", interceptor.connect_params.http_username);
    HTTP_LOG_I("password:%s", interceptor.connect_params.http_password);
    HTTP_LOG_I("path:%s", interceptor.connect_params.http_path);
    HTTP_LOG_I("query:%s", interceptor.connect_params.http_query);
    HTTP_LOG_I("farg:%s\n", interceptor.connect_params.http_farg);

    sleep(2);

    return 0;
}