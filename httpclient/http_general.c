/*
 * @Author: jiejie
 * @Github: https://github.com/jiejieTop
 * @Date: 2020-05-08 21:08:26
 * @LastEditTime: 2020-06-01 23:51:51
 * @Description: the code belongs to jiejie, please keep the author information and source code according to the license.
 */

#include <http_log.h>
#include <http_error.h>
#include <platform_memory.h>
#include <http_utils.h>
#include <http_general.h>

HTTP_SET_AND_GET_CONNECT_PARAMS_DEFINE(url, const char*, NULL)
HTTP_SET_AND_GET_CONNECT_PARAMS_DEFINE(scheme, const char*, NULL)
HTTP_SET_AND_GET_CONNECT_PARAMS_DEFINE(host, const char*, NULL)
HTTP_SET_AND_GET_CONNECT_PARAMS_DEFINE(user, const char*, NULL)
HTTP_SET_AND_GET_CONNECT_PARAMS_DEFINE(password, const char*, NULL)
HTTP_SET_AND_GET_CONNECT_PARAMS_DEFINE(path, const char*, NULL)
HTTP_SET_AND_GET_CONNECT_PARAMS_DEFINE(query, const char*, NULL)
HTTP_SET_AND_GET_CONNECT_PARAMS_DEFINE(farg, const char*, NULL)
HTTP_SET_AND_GET_CONNECT_PARAMS_DEFINE(port, const char*, NULL)

http_connect_params_t *http_assign_connect_params(void)
{
    int len = sizeof(http_connect_params_t);
    http_connect_params_t *connect_params = NULL;

    connect_params = platform_memory_alloc(len);
    memset(connect_params, 0, len);

    return connect_params;
}

void http_release_connect_params_variables(http_connect_params_t *connect_params)
{
    HTTP_FREE_ALL_CONNECT_PARAMS(connect_params);
}

void http_release_connect_params(http_connect_params_t *connect_params)
{
    if (NULL != connect_params) {
        HTTP_FREE_ALL_CONNECT_PARAMS(connect_params);
        platform_memory_free(connect_params);
        connect_params = NULL;
    }
}



