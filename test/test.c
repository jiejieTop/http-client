/*
 * @Author: jiejie
 * @Github: https://github.com/jiejieTop
 * @Date: 2019-12-11 21:53:07
 * @LastEditTime: 2020-05-08 22:44:29
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
#define URL1 "https://jiejie:test1@jiedev1.com/test1/index.php?who=jiejie1&is=test#frag1"


int main(void)
{
    http_interceptor_t interceptor;
    http_connect_params_t *connect_params = NULL;

    memset(&interceptor, 0, sizeof(http_interceptor_t));

    http_log_init();

    // HTTP_LOG_I("url:%s", URL);
    
    connect_params = http_assign_connect_params();

    http_url_parsing(connect_params, URL);

    HTTP_LOG_I("host:%s", http_get_connect_params_host(connect_params));
    HTTP_LOG_I("port:%s", http_get_connect_params_port(connect_params));
    HTTP_LOG_I("scheme:%s", http_get_connect_params_scheme(connect_params));
    HTTP_LOG_I("user:%s", http_get_connect_params_user(connect_params));
    HTTP_LOG_I("password:%s", http_get_connect_params_password(connect_params));
    HTTP_LOG_I("path:%s", http_get_connect_params_path(connect_params));
    HTTP_LOG_I("query:%s", http_get_connect_params_query(connect_params));
    HTTP_LOG_I("farg:%s\n", http_get_connect_params_farg(connect_params));

    

    // http_request_init(&interceptor.request);
    // http_request_header_init(&interceptor.request);

    // http_request_add_header_form_index(&interceptor.request, HTTP_REQUEST_HEADER_HOST, http_get_connect_params_host(connect_params));
    
    // http_request_set_version(&interceptor.request, "HTTP/2.0");
    // HTTP_LOG_I("host:%s", http_request_get_header_form_index(&interceptor.request, HTTP_REQUEST_HEADER_HOST));

    // http_request_set_method(&interceptor.request, HTTP_REQUEST_METHOD_GET);
    // http_request_set_start_line_with_query(&interceptor.request, connect_params->http_path, connect_params->http_query);

    // http_request_set_body(&interceptor.request, connect_params->http_url, strlen(connect_params->http_url));

    // http_request_print_start_line(&interceptor.request);
    // http_request_print_header(&interceptor.request);
    // http_request_print_body(&interceptor.request);

    http_interceptor_init(&interceptor);
    http_interceptor_set_connect_params(&interceptor, connect_params);
    http_interceptor_connect(&interceptor);
    http_interceptor_request(&interceptor, HTTP_REQUEST_METHOD_GET);


    sleep(5);

    return 0;
}