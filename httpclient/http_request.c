/*
 * @Author: jiejie
 * @Github: https://github.com/jiejieTop
 * @Date: 2020-05-05 17:20:36
 * @LastEditTime: 2020-05-05 21:52:49
 * @Description: the code belongs to jiejie, please keep the author information and source code according to the license.
 */

#include <http_request.h>
#include <http_log.h>
#include <http_error.h>

static const char *HTTP_METHOD_MAPPING[] = {"INVALID ", "GET ", "POST ", "HEAD ", "PUT ", "DELETE ", "OPTIONS ", "TRACE ", "CONNECT ", "PATCH " };

static const char *HTTP_REQUEST_HEADERS_MAPPING[] = {
    "INVALID",
    "Cache-Control: ",
    "Connection: ",
    "Date: ",
    "Pragma: ",
    "Trailer: ",
    "Transfer-Encoding: ",
    "Upgrade: ",
    "Via: ",
    "Warning: ",
    "Accept: ",
    "Accept-Charset: ",
    "Accept-Encoding: ",
    "Accept-Language: ",
    "Authorization: ",
    "Expect: ",
    "From: ",
    "Host: ",
    "If-Match: ",
    "If-Modified-Since: ",
    "If-None-Match: ",
    "If-Range: ",
    "If-Unmodified-Since: ",
    "Max-Forwards: ",
    "Proxy-Authorization: ",
    "Range: ",
    "Referer: ",
    "TE: ",
    "User-Agent: "
};

int http_request_init(http_request_t *req)
{
    HTTP_ROBUSTNESS_CHECK(req , HTTP_NULL_VALUE_ERROR);

    req->req_msg.line = http_message_buffer_init(HTTP_MESSAGE_BUFFER_GROWTH);
    req->req_msg.header = http_message_buffer_init(HTTP_MESSAGE_BUFFER_GROWTH);
    req->req_msg.body = http_message_buffer_init(HTTP_MESSAGE_BUFFER_GROWTH);
}


int http_request_header_init(http_request_t *req)
{
    HTTP_ROBUSTNESS_CHECK(req , HTTP_NULL_VALUE_ERROR);

    if (!req->req_flag.flag_t.no_keep_alive) {
        http_request_add_header_form_index(req, HTTP_REQUEST_HEADER_CONNECTION, "Keep-Alive");
    }

    http_request_add_header_form_index(req, HTTP_REQUEST_HEADER_CONNECTION, "Keep-Alive");

    printf("%s\n", req->req_msg.header->data);
}

int http_request_start_line(http_request_t *req,  http_request_method_t method, const char *path)
{
    HTTP_ROBUSTNESS_CHECK((req && method && path), HTTP_NULL_VALUE_ERROR);

    http_request_init(req);

    http_request_header_init(req);
    
    const char *m = HTTP_METHOD_MAPPING[method];
    
    http_message_buffer_concat(req->req_msg.line, m, path, HTTP_DEFAULT_VERSION, HTTP_CRLF, NULL);

    
    printf("start line: %s", req->req_msg.line->data);


}

/* adds a header to the request with given key and value. */
void http_request_add_header(http_request_t *req, const char *key, const char *value)
{
    http_message_buffer_concat(req->req_msg.header, key, ": ", value, HTTP_CRLF, NULL);
}

/* adds a header to the request with given key and value. */
void http_request_add_header_form_index(http_request_t *req, http_request_header_t header, const char *value)
{
    const char *key = HTTP_REQUEST_HEADERS_MAPPING[header];

    http_message_buffer_concat(req->req_msg.header, key, value, HTTP_CRLF, NULL);
}

char http_request_get_header(http_request_t *req, const char *key)
{
    
}


