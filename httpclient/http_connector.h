/*
 * @Author: jiejie
 * @Github: https://github.com/jiejieTop
 * @Date: 2020-04-16 20:36:06
 * @LastEditTime: 2020-05-03 23:37:46
 * @Description: the code belongs to jiejie, please keep the author information and source code according to the license.
 */

#ifndef _HTTP_CONNECTOR_H_
#define _HTTP_CONNECTOR_H_

#include <http_list.h>
#include <http_log.h>
#include <http_utils.h>
#include <http_interceptor.h>

#ifndef HTTP_CONNECT_POOL_MAX_NUM
    #define HTTP_CONNECT_POOL_MAX_NUM   4
#endif

#define HTTP_GET_CONNECT_PARAMS_DEFINE(name)                                            \
    char *http_get_connect_params_##name(http_connect_params_t *connect_params) {       \
        HTTP_ROBUSTNESS_CHECK((connect_params), NULL);                                  \
        return connect_params->http_##name;                                             \
    }

#define HTTP_SET_CONNECT_PARAMS_DEFINE(name)                                                    \
    char *http_set_connect_params_##name(http_connect_params_t *connect_params, char *ptr) {    \
        HTTP_ROBUSTNESS_CHECK((connect_params && ptr), NULL);                                   \
        connect_params->http_##name = ptr;                                                      \
        return connect_params->http_##name;                                                     \
    }

#define HTTP_GET_CONNECT_PARAMS_STATEMENT(name)                                         \
    char *http_get_connect_params_##name(http_connect_params_t *);

#define HTTP_SET_CONNECT_PARAMS_STATEMENT(name)                                         \
    char *http_set_connect_params_##name(http_connect_params_t *, char *);

#define HTTP_SET_AND_GET_CONNECT_PARAMS_DEFINE(name)       \
    HTTP_SET_CONNECT_PARAMS_DEFINE(name)                   \
    HTTP_GET_CONNECT_PARAMS_DEFINE(name)

#define HTTP_SET_AND_GET_CONNECT_PARAMS_STATEMENT(name)    \
    HTTP_SET_CONNECT_PARAMS_STATEMENT(name)                \
    HTTP_GET_CONNECT_PARAMS_STATEMENT(name)

typedef struct http_response {
	struct parsed_url *request_uri;
	char *body;
	char *status_code;
	int status_code_int;
	char *status_text;
	char *request_headers;
	char *response_headers;
} http_response_t;


typedef enum http_connect_status {
    HTTP_CONNECT_STATUS_IDLE    =   0x00,   /* is idle */
    HTTP_CONNECT_STATUS_LEASE   =   0x01,   /* is lease */
    HTTP_CONNECT_STATUS_CONNECT =   0x02,   /* is connect */
    HTTP_CONNECT_STATUS_STABLE  =   0x04,   /* is stable */
    HTTP_CONNECT_STATUS_STALE   =   0x08,   /* is stale */
    HTTP_CONNECT_STATUS_DESTROY =   0x10    /* is destroy */
} http_connect_status_t;


typedef struct http_client {
    http_connect_status_t               status;
    http_list_t                         connect_pool_list;
    // network_t                   *network;
    
} http_client_t;


HTTP_SET_AND_GET_CONNECT_PARAMS_STATEMENT(url)
HTTP_SET_AND_GET_CONNECT_PARAMS_STATEMENT(scheme)
HTTP_SET_AND_GET_CONNECT_PARAMS_STATEMENT(host)
HTTP_SET_AND_GET_CONNECT_PARAMS_STATEMENT(username)
HTTP_SET_AND_GET_CONNECT_PARAMS_STATEMENT(password)
HTTP_SET_AND_GET_CONNECT_PARAMS_STATEMENT(path)
HTTP_SET_AND_GET_CONNECT_PARAMS_STATEMENT(query)
HTTP_SET_AND_GET_CONNECT_PARAMS_STATEMENT(farg)

uint16_t http_get_connect_params_port(http_connect_params_t *connect_params);
uint16_t http_set_connect_params_port(http_connect_params_t *connect_params, uint16_t port);

http_connect_params_t *http_assign_connect_params(void);
void http_release_connect_params(http_connect_params_t *connect_params);

#endif // !_HTTP_CONNECTOR_H_

