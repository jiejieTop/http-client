/*
 * @Author: jiejie
 * @Github: https://github.com/jiejieTop
 * @Date: 2020-04-16 20:31:12
 * @LastEditTime: 2020-06-02 20:33:32
 * @Description: the code belongs to jiejie, please keep the author information and source code according to the license.
 */

#ifndef _HTTP_INTERCEPTOR_H_
#define _HTTP_INTERCEPTOR_H_

#include <network.h>
#include <http_parser.h>
#include <http_request.h>
#include <http_response.h>
#include <http_general.h>
#include <http_event.h>

typedef enum http_interceptor_status {
    http_interceptor_status_invalid = 0x00,
    http_interceptor_status_init,
    http_interceptor_status_connect,
    http_interceptor_status_request,
    http_interceptor_status_response_headers,
    http_interceptor_status_headers_complete,
    http_interceptor_status_response_body,
    http_interceptor_status_response_complete,
    http_interceptor_status_release
} http_interceptor_status_t;

typedef struct http_interceptor {
    network_t                   *network;
    http_connect_params_t       *connect_params;
    http_request_t              request;
    http_response_t             response;
    http_interceptor_status_t   status;
    http_message_buffer_t       *message;
    char                        *buffer;
    size_t                      buffer_len;
    size_t                      cmd_timeout;
    size_t                      data_process;
    struct http_parser          *parser;
    struct http_parser_settings *parser_settings;
    http_event_t                *evetn;
    void                        *owner;
    HTTP_GENERAL_FLAG;
} http_interceptor_t;


int http_interceptor_init(http_interceptor_t *interceptor);
void http_interceptor_set_ca(const char *ca);
void http_interceptor_set_owner(http_interceptor_t *interceptor, void *owner);
void http_interceptor_event_register(http_interceptor_t *interceptor, http_event_cb_t cb);
int http_interceptor_connect(http_interceptor_t *interceptor);
void http_interceptor_set_keep_alive(http_interceptor_t *interceptor);
int http_interceptor_set_connect_params(http_interceptor_t *interceptor, http_connect_params_t *conn_param);
int http_interceptor_request(http_interceptor_t *interceptor, http_request_method_t mothod, const char *post_buf);
int http_interceptor_release(http_interceptor_t *interceptor);
int http_interceptor_process(http_interceptor_t *interceptor,
                             http_connect_params_t *connect_params,
                             http_request_method_t mothod, 
                             void *post_buf,
                             void *owner,
                             http_event_cb_t cb);

#endif // !_HTTP_INTERCEPTOR_H_
