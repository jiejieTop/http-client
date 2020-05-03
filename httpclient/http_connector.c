/*
 * @Author: jiejie
 * @Github: https://github.com/jiejieTop
 * @Date: 2020-04-16 20:36:06
 * @LastEditTime: 2020-05-03 23:25:28
 * @Description: the code belongs to jiejie, please keep the author information and source code according to the license.
 */

#include <http_connector.h>
#include <http_log.h>
#include <http_error.h>
#include <platform_memory.h>


http_list_t _http_idle_list;
http_list_t _http_lease_list;
http_list_t _http_stable_list;


HTTP_SET_AND_GET_CONNECT_PARAMS_DEFINE(url)
HTTP_SET_AND_GET_CONNECT_PARAMS_DEFINE(scheme)
HTTP_SET_AND_GET_CONNECT_PARAMS_DEFINE(host)
HTTP_SET_AND_GET_CONNECT_PARAMS_DEFINE(username)
HTTP_SET_AND_GET_CONNECT_PARAMS_DEFINE(password)
HTTP_SET_AND_GET_CONNECT_PARAMS_DEFINE(path)
HTTP_SET_AND_GET_CONNECT_PARAMS_DEFINE(query)
HTTP_SET_AND_GET_CONNECT_PARAMS_DEFINE(farg)


uint16_t http_get_connect_params_port(http_connect_params_t *connect_params)
{
    HTTP_ROBUSTNESS_CHECK((connect_params), 0);
    return connect_params->http_port; 
}

uint16_t http_set_connect_params_port(http_connect_params_t *connect_params, uint16_t port)
{
    HTTP_ROBUSTNESS_CHECK((connect_params && port), 0);
    connect_params->http_port = port;
    return connect_params->http_port; 
}


http_connect_params_t *http_assign_connect_params(void)
{
    http_connect_params_t *connect_params = NULL;
    connect_params = platform_memory_alloc(sizeof(http_connect_params_t));
    return connect_params;
}


void http_release_connect_params(http_connect_params_t *connect_params)
{
    if (NULL != connect_params)
        platform_memory_free(connect_params);
}











