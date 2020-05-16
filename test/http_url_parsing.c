/*
 * @Author: jiejie
 * @Github: https://github.com/jiejieTop
 * @Date: 2020-05-16 22:23:53
 * @LastEditTime: 2020-05-16 22:48:41
 * @Description: the code belongs to jiejie, please keep the author information and source code according to the license.
 */ 
#include <stdio.h>
#include <unistd.h>
#include <httpclient.h>

#define URL "http://jiejie:test@jiedev.com/index.php?who=jiejie&is=test#frag"

void http_url_parsing_test(void)
{
    printf("\n---------------------- http_url_parsing_test start ----------------------\n");

    http_connect_params_t *connect_params = NULL;
    
    connect_params = http_assign_connect_params();

    http_url_parsing(connect_params, URL);

    printf("url:%s\n", http_get_connect_params_url(connect_params));
    printf("host:%s\n", http_get_connect_params_host(connect_params));
    printf("port:%s\n", http_get_connect_params_port(connect_params));
    printf("scheme:%s\n", http_get_connect_params_scheme(connect_params));
    printf("user:%s\n", http_get_connect_params_user(connect_params));
    printf("password:%s\n", http_get_connect_params_password(connect_params));
    printf("path:%s\n", http_get_connect_params_path(connect_params));
    printf("query:%s\n", http_get_connect_params_query(connect_params));
    printf("farg:%s\n", http_get_connect_params_farg(connect_params));

    http_release_connect_params(connect_params);

    printf("\n---------------------- http_url_parsing_test end ----------------------\n");
    
}
