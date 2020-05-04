/*
 * @Author: jiejie
 * @Github: https://github.com/jiejieTop
 * @Date: 2020-04-16 20:31:12
 * @LastEditTime: 2020-05-04 18:55:46
 * @Description: the code belongs to jiejie, please keep the author information and source code according to the license.
 */

#ifndef _HTTP_INTERCEPTOR_H_
#define _HTTP_INTERCEPTOR_H_

#include <network.h>
#include <http_parser.h>

typedef enum http_request_method {
    HTTP_REQUEST_METHOD_INVALID      =   0x00,
    HTTP_REQUEST_METHOD_GET          =   0x01,       /* http GET method, http/1.0 */
    HTTP_REQUEST_METHOD_POST         =   0x02,       /* http POST method, http/1.0 */
    HTTP_REQUEST_METHOD_HEAD         =   0x03,       /* http HEAD method, http/1.0 */
    HTTP_REQUEST_METHOD_PUT          =   0x04,       /* http PUT method, http/1.1 */
    HTTP_REQUEST_METHOD_DELETE       =   0x05,       /* http DELETE method, http/1.1 */
    HTTP_REQUEST_METHOD_OPTIONS      =   0x06,       /* http OPTIONS method, http/1.1 */
    HTTP_REQUEST_METHOD_TRACE        =   0x07,       /* http TRACE method, http/1.1 */
    HTTP_REQUEST_METHOD_CONNECT      =   0x08,       /* http CONNECT method, http/1.1, reserved */
    HTTP_REQUEST_METHOD_PATCH        =   0x09        /* http PATCH method, echo test */
} http_request_method_t;

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

typedef struct http_connect_params {
    char                        *http_url;
    char                        *http_scheme;
    char                        *http_host;
    char                        *http_username;
    char                        *http_password;
    char                        *http_path;
    char                        *http_query;
    char                        *http_farg;
    char                        *http_cert_pem;
    uint16_t                    http_port;
    http_request_method_t       http_method;
} http_connect_params_t;



typedef struct http_interceptor {
    network_t                   *network;
    network_params_t            network_params;
    http_connect_params_t       connect_params;
    struct http_parser          *parser;
    struct http_parser_settings *parser_settings;
    
} http_interceptor_t;




#endif // !_HTTP_INTERCEPTOR_H_
