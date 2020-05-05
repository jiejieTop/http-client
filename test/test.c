/*
 * @Author: jiejie
 * @Github: https://github.com/jiejieTop
 * @Date: 2019-12-11 21:53:07
 * @LastEditTime: 2020-05-05 21:47:47
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

#define URL "http://jiejie:test@jiedev.com/test/index.php?who=jiejie&is=test#frag"
#define URL1 "https://jiejie:test1@jiedev1.com/test1/index.php?who=jiejie1&is=test#frag1"


int main(void)
{
    http_interceptor_t interceptor;

    memset(&interceptor, 0, sizeof(http_interceptor_t));

    http_log_init();

    HTTP_LOG_I("url:%s", URL);
    
    http_url_parsing(&interceptor.connect_params, URL);

    HTTP_LOG_I("host:%s", interceptor.connect_params.http_host);
    HTTP_LOG_I("port:%d", interceptor.connect_params.http_port);
    HTTP_LOG_I("scheme:%s", interceptor.connect_params.http_scheme);
    HTTP_LOG_I("username:%s", interceptor.connect_params.http_username);
    HTTP_LOG_I("password:%s", interceptor.connect_params.http_password);
    HTTP_LOG_I("path:%s", interceptor.connect_params.http_path);
    HTTP_LOG_I("query:%s", interceptor.connect_params.http_query);
    HTTP_LOG_I("farg:%s\n", interceptor.connect_params.http_farg);

    // http_url_parsing(&interceptor.connect_params, URL1);

    // HTTP_LOG_I("host:%s", interceptor.connect_params.http_host);
    // HTTP_LOG_I("port:%d", interceptor.connect_params.http_port);
    // HTTP_LOG_I("scheme:%s", interceptor.connect_params.http_scheme);
    // HTTP_LOG_I("username:%s", interceptor.connect_params.http_username);
    // HTTP_LOG_I("password:%s", interceptor.connect_params.http_password);
    // HTTP_LOG_I("path:%s", interceptor.connect_params.http_path);
    // HTTP_LOG_I("query:%s", interceptor.connect_params.http_query);
    // HTTP_LOG_I("farg:%s\n", interceptor.connect_params.http_farg);

    http_request_start_line(&interceptor.request, HTTP_REQUEST_METHOD_GET, interceptor.connect_params.http_path);
    
    sleep(2);

    return 0;
}