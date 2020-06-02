/*
 * @Author: jiejie
 * @Github: https://github.com/jiejieTop
 * @Date: 2020-05-05 19:36:42
 * @LastEditTime: 2020-06-02 23:01:24
 * @Description: the code belongs to jiejie, please keep the author information and source code according to the license.
 */

#ifndef _HTTP_RESPONSE_H_
#define _HTTP_RESPONSE_H_

#include <http_general.h>
#include <http_message_buffer.h>


typedef enum http_response_status {
    http_response_status_continue                           = 100,      /* informational 1xx  */
    http_response_status_switching_protocols                = 101,
    http_response_status_ok                                 = 200,      /* successful 2xx */
    http_response_status_created                            = 201,
    http_response_status_accepted                           = 202,
    http_response_status_non_authoritative_information      = 203,
    http_response_status_no_content                         = 204,
    http_response_status_reset_content                      = 205,
    http_response_status_partial_content                    = 206,
    http_response_status_multiple_choices                   = 300,      /* redirection 3xx */
    http_response_status_moved_permanently                  = 301,
    http_response_status_found                              = 302,
    http_response_status_see_ther                           = 303,
    http_response_status_not_modified                       = 304,
    http_response_status_use_proxy                          = 305,
    http_response_status_unused                             = 306,      /* unused */
    http_response_status_temporary_redirect                 = 307,
    http_response_status_bad_request                        = 400,      /* client error 4xx */  
    http_response_status_unauthorized                       = 401,
    http_response_status_payment_required                   = 402,      /* this code is reserved for future use */
    http_response_status_forbidden                          = 403,
    http_response_status_not_found                          = 404,
    http_response_status_method_not_allowed                 = 405,
    http_response_status_not_acceptable                     = 406,
    http_response_status_proxy_authentication_required      = 407,
    http_response_status_request_timeout                    = 408,
    http_response_status_conflict                           = 409,
    http_response_status_gone                               = 410,
    http_response_status_length_required                    = 411,
    http_response_status_precondition_failed                = 412,
    http_response_status_request_entity_too_large           = 413,
    http_response_status_request_uri_too_long               = 414,
    http_response_status_unsupported_media_type             = 415,
    http_response_status_requested_range_not_satisfiable    = 416,
    http_response_status_expectation_failed                 = 417,
    http_response_status_internal_server_error              = 500,         /* server error 5xx */
    http_response_status_not_implemented                    = 501,
    http_response_status_bad_gateway                        = 502,
    http_response_status_service_unavailable                = 503,
    http_response_status_gateway_timeout                    = 504,
    http_response_status_http_version_not_supported         = 505,
} http_response_status_t;


typedef struct http_response {
    size_t                          length;         /* content length */
    uint32_t                        offset;         /* data offset, skip header */
    http_response_status_t          status;         /* response status code */
} http_response_t;

int http_response_init(http_response_t *rsp);

size_t http_response_get_length(http_response_t *rsp);
void http_response_set_length(http_response_t *rsp, size_t length);

uint32_t http_response_get_offset(http_response_t *rsp);
void http_response_set_offset(http_response_t *rsp, uint32_t offset);

http_response_status_t http_response_get_status(http_response_t *rsp);
void http_response_set_status(http_response_t *rsp, unsigned int status);


#endif // !_HTTP_RESPONSE_H_
