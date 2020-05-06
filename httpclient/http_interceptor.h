/*
 * @Author: jiejie
 * @Github: https://github.com/jiejieTop
 * @Date: 2020-04-16 20:31:12
 * @LastEditTime: 2020-05-06 09:08:26
 * @Description: the code belongs to jiejie, please keep the author information and source code according to the license.
 */

#ifndef _HTTP_INTERCEPTOR_H_
#define _HTTP_INTERCEPTOR_H_

#include <network.h>
#include <http_parser.h>
#include <http_request.h>
#include <http_url_parser.h>

// typedef struct {
//     http_header_handle_t headers;       /*!< http header */
//     esp_http_buffer_t   *buffer;        /*!< data buffer as linked list */
//     int                 status_code;    /*!< status code (integer) */
//     int                 content_length; /*!< data length */
//     int                 data_offset;    /*!< offset to http data (Skip header) */
//     int                 data_process;   /*!< data processed */
//     int                 method;         /*!< http method */
//     bool                is_chunked;
// } http_data_t;



typedef struct http_interceptor {
    network_t                   *network;
    network_params_t            network_params;
    http_connect_params_t       connect_params;
    http_request_t              request;
    struct http_parser          *parser;
    struct http_parser_settings *parser_settings;
    
} http_interceptor_t;



#endif // !_HTTP_INTERCEPTOR_H_
