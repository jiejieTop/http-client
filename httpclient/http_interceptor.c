/*
 * @Author: jiejie
 * @Github: https://github.com/jiejieTop
 * @Date: 2020-04-16 20:31:12
 * @LastEditTime: 2020-05-03 23:29:28
 * @Description: the code belongs to jiejie, please keep the author information and source code according to the license.
 */

#include <http_interceptor.h>
#include <http_log.h>
#include <http_error.h>
#include <http_utils.h>
#include <http_url_parser.h>
#include <http_connector.h>

int http_interceptor_connect_request(http_interceptor_t *interceptor, const char* url, http_request_method_t method)
{
    int res;
    
    res = http_url_parsing(&interceptor->connect_params, url);

    HTTP_LOG_I("host:%s", http_get_connect_params_host(&interceptor->connect_params));
    HTTP_LOG_I("port:%d", http_get_connect_params_port(&interceptor->connect_params));
    HTTP_LOG_I("scheme:%s", http_get_connect_params_scheme(&interceptor->connect_params));
    HTTP_LOG_I("username:%s", http_get_connect_params_username(&interceptor->connect_params));
    HTTP_LOG_I("password:%s", http_get_connect_params_password(&interceptor->connect_params));
    HTTP_LOG_I("path:%s", http_get_connect_params_path(&interceptor->connect_params));
    HTTP_LOG_I("query:%s", http_get_connect_params_query(&interceptor->connect_params));
    HTTP_LOG_I("farg:%s\n", http_get_connect_params_farg(&interceptor->connect_params));


    RETURN_ERROR(HTTP_SUCCESS_ERROR);
}