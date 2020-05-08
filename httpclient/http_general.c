/*
 * @Author: jiejie
 * @Github: https://github.com/jiejieTop
 * @Date: 2020-05-08 21:08:26
 * @LastEditTime: 2020-05-08 21:15:13
 * @Description: the code belongs to jiejie, please keep the author information and source code according to the license.
 */

#include <http_log.h>
#include <http_error.h>
#include <platform_memory.h>
#include <http_utils.h>
#include <http_general.h>

HTTP_SET_AND_GET_CONNECT_PARAMS_DEFINE(url, char*, NULL)
HTTP_SET_AND_GET_CONNECT_PARAMS_DEFINE(scheme, char*, NULL)
HTTP_SET_AND_GET_CONNECT_PARAMS_DEFINE(host, char*, NULL)
HTTP_SET_AND_GET_CONNECT_PARAMS_DEFINE(user, char*, NULL)
HTTP_SET_AND_GET_CONNECT_PARAMS_DEFINE(password, char*, NULL)
HTTP_SET_AND_GET_CONNECT_PARAMS_DEFINE(path, char*, NULL)
HTTP_SET_AND_GET_CONNECT_PARAMS_DEFINE(query, char*, NULL)
HTTP_SET_AND_GET_CONNECT_PARAMS_DEFINE(farg, char*, NULL)
HTTP_SET_AND_GET_CONNECT_PARAMS_DEFINE(port, char*, NULL)

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



