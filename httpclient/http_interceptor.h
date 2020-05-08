/*
 * @Author: jiejie
 * @Github: https://github.com/jiejieTop
 * @Date: 2020-04-16 20:31:12
 * @LastEditTime: 2020-05-08 22:18:50
 * @Description: the code belongs to jiejie, please keep the author information and source code according to the license.
 */

#ifndef _HTTP_INTERCEPTOR_H_
#define _HTTP_INTERCEPTOR_H_

#include <network.h>
#include <http_parser.h>
#include <http_request.h>
#include <http_general.h>

typedef enum http_interceptor_status {
    http_interceptor_status_invalid = 0x00,
    http_interceptor_status_init,
    http_interceptor_status_connect,
    http_interceptor_status_request,
    http_interceptor_status_post,
    http_interceptor_status_response_headers,
    http_interceptor_status_response_body,
    http_interceptor_status_close
} http_interceptor_status_t;

typedef struct http_interceptor {
    network_t                   *network;
    http_connect_params_t       *connect_params;
    http_request_t              request;
    http_interceptor_status_t   status;
    
    unsigned int                cmd_timeout;
    struct http_parser          *parser;
    struct http_parser_settings *parser_settings;
    
} http_interceptor_t;


int http_interceptor_init(http_interceptor_t *interceptor);
int http_interceptor_connect(http_interceptor_t *interceptor);
int http_interceptor_set_connect_params(http_interceptor_t *interceptor, http_connect_params_t *conn_param);
int http_interceptor_request(http_interceptor_t *interceptor, http_request_method_t mothod);

#endif // !_HTTP_INTERCEPTOR_H_
