/*
 * @Author: jiejie
 * @Github: https://github.com/jiejieTop
 * @Date: 2020-05-05 17:20:42
 * @LastEditTime: 2020-06-02 17:41:32
 * @Description: the code belongs to jiejie, please keep the author information and source code according to the license.
 */
#ifndef _HTTP_REQUEST_H_
#define _HTTP_REQUEST_H_

#include <stdint.h>
#include <http_general.h>
#include <http_message_buffer.h>


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

typedef enum http_request_header {
    HTTP_REQUEST_HEADER_INVALID = 0x00,
    HTTP_REQUEST_HEADER_CACHE_CONTROL,
    HTTP_REQUEST_HEADER_CONNECTION,
    HTTP_REQUEST_HEADER_DATE,
    HTTP_REQUEST_HEADER_PRAGMA,
    HTTP_REQUEST_HEADER_TRAILER,
    HTTP_REQUEST_HEADER_TRANSFER_ENCODING,
    HTTP_REQUEST_HEADER_UPGRADE,
    HTTP_REQUEST_HEADER_VIA,
    HTTP_REQUEST_HEADER_WARNING,
    HTTP_REQUEST_HEADER_ACCEPT,
    HTTP_REQUEST_HEADER_ACCEPT_CHARSET,
    HTTP_REQUEST_HEADER_ACCEPT_ENCODING,
    HTTP_REQUEST_HEADER_ACCEPT_LANGUAGE,
    HTTP_REQUEST_HEADER_AUTHORIZATION,
    HTTP_REQUEST_HEADER_EXPECT,
    HTTP_REQUEST_HEADER_FROM,
    HTTP_REQUEST_HEADER_HOST,
    HTTP_REQUEST_HEADER_IF_MATCH,
    HTTP_REQUEST_HEADER_IF_MODIFIED_SINCE,
    HTTP_REQUEST_HEADER_IF_NONE_MATCH,
    HTTP_REQUEST_HEADER_IF_RANGE,
    HTTP_REQUEST_HEADER_IF_UNMODIFIED_SINCE,
    HTTP_REQUEST_HEADER_MAX_FORWARDS,
    HTTP_REQUEST_HEADER_PROXY_AUTHORIZATION,
    HTTP_REQUEST_HEADER_RANGE,
    HTTP_REQUEST_HEADER_REFERER,
    HTTP_REQUEST_HEADER_TE,
    HTTP_REQUEST_HEADER_USER_AGENT,
    HTTP_REQUEST_HEADER_ALLOW,
    HTTP_REQUEST_HEADER_CONTENT_LENGTH,
    HTTP_REQUEST_HEADER_CONTENT_TYPE
} http_request_header_t;

typedef struct http_request_message {
    http_message_buffer_t           *line;
    http_message_buffer_t           *header;
    http_message_buffer_t           *body;
} http_request_message_t;

typedef struct http_request {
    http_request_method_t           method;
    http_request_message_t          req_msg;
    uint32_t                        header_index;
    HTTP_GENERAL_FLAG;
} http_request_t;

int http_request_init(http_request_t *req);
int http_request_release(http_request_t *req);
int http_request_set_method(http_request_t *req,  http_request_method_t method);
int http_request_set_start_line(http_request_t *req, const char *path);
int http_request_set_start_line_with_query(http_request_t *req, const char *path, const char *query);
int http_request_header_init(http_request_t *req);
void http_request_add_header(http_request_t *req, const char *key, const char *value);
void http_request_add_header_form_index(http_request_t *req, http_request_header_t index, const char *value);
char *http_request_get_header(http_request_t *req, const char *key);
char *http_request_get_header_form_index(http_request_t *req, http_request_header_t index);

int http_request_set_body(http_request_t *req, const char *buf, size_t size);
int http_request_set_body_form_pointer(http_request_t *req, const char *buf, size_t size);

int http_request_set_keep_alive(http_request_t *req);
int http_request_set_version(http_request_t *req, const char *str);

char *http_request_get_start_line_data(http_request_t *req);
char *http_request_get_header_data(http_request_t *req);
char *http_request_get_body_data(http_request_t *req);

size_t http_request_get_start_line_length(http_request_t *req);
size_t http_request_get_header_length(http_request_t *req);
size_t http_request_get_body_length(http_request_t *req);

void http_request_print_start_line(http_request_t *req);
void http_request_print_header(http_request_t *req);
void http_request_print_body(http_request_t *req);

#endif // !_HTTP_REQUEST_H_
