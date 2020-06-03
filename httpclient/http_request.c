/*
 * @Author: jiejie
 * @Github: https://github.com/jiejieTop
 * @Date: 2020-05-05 17:20:36
 * @LastEditTime: 2020-06-03 19:32:43
 * @Description: the code belongs to jiejie, please keep the author information and source code according to the license.
 */

#include <http_request.h>
#include <http_log.h>
#include <http_error.h>

static const char *_http_request_version = HTTP_DEFAULT_VERSION;

static const char *_http_method_mapping[] = {"INVALID ", "GET ", "POST ", "HEAD ", "PUT ", "DELETE ", "OPTIONS ", "TRACE ", "CONNECT ", "PATCH " };

static const char *_http_request_headers_mapping[] = {
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
    "User-Agent: ",
    "Allow: ",
    "Content-Length: ",
    "Content-Type: "
};

int http_request_init(http_request_t *req)
{
    HTTP_ROBUSTNESS_CHECK(req , HTTP_NULL_VALUE_ERROR);

    req->req_msg.line = http_message_buffer_init(HTTP_MESSAGE_BUFFER_GROWTH);
    req->req_msg.header = http_message_buffer_init(HTTP_MESSAGE_BUFFER_GROWTH);
    req->req_msg.body = http_message_buffer_init(HTTP_MESSAGE_BUFFER_GROWTH);

    RETURN_ERROR(HTTP_SUCCESS_ERROR);
}

int http_request_release(http_request_t *req)
{
    HTTP_ROBUSTNESS_CHECK(req , HTTP_NULL_VALUE_ERROR);

    http_message_buffer_release(req->req_msg.line );
    http_message_buffer_release(req->req_msg.header);
    http_message_buffer_release(req->req_msg.body);

    req->req_msg.line = NULL;
    req->req_msg.header = NULL;
    req->req_msg.body = NULL;
    
    RETURN_ERROR(HTTP_SUCCESS_ERROR);
}


int http_request_header_init(http_request_t *req)
{
    HTTP_ROBUSTNESS_CHECK(req , HTTP_NULL_VALUE_ERROR);

    req->header_index = 0;
    http_message_buffer_reinit(req->req_msg.header);

    http_request_add_header_form_index(req, HTTP_REQUEST_HEADER_USER_AGENT, "http-client-by-jiejie");
    http_request_add_header_form_index(req, HTTP_REQUEST_HEADER_ACCEPT, "*/*");

    if (req->flag.flag_t.keep_alive) {
        http_request_add_header_form_index(req, HTTP_REQUEST_HEADER_CONNECTION, "keep-alive");
    } else {
        http_request_add_header_form_index(req, HTTP_REQUEST_HEADER_CONNECTION, "Close");
    }

    RETURN_ERROR(HTTP_SUCCESS_ERROR);
}


int http_request_set_version(http_request_t *req, const char *str)
{
    HTTP_ROBUSTNESS_CHECK((req && str) , HTTP_NULL_VALUE_ERROR);
    if (http_utils_nmatch(str, "HTTP/", 5) == 0) {
        _http_request_version = str;
    } else {
        RETURN_ERROR(HTTP_NULL_VALUE_ERROR);
    }

    RETURN_ERROR(HTTP_SUCCESS_ERROR);
}

int http_request_set_keep_alive(http_request_t *req)
{
    HTTP_ROBUSTNESS_CHECK(req , HTTP_NULL_VALUE_ERROR);
    req->flag.flag_t.keep_alive = 1;
    RETURN_ERROR(HTTP_SUCCESS_ERROR);
}

int http_request_set_method(http_request_t *req,  http_request_method_t method)
{
    HTTP_ROBUSTNESS_CHECK((req && method), HTTP_NULL_VALUE_ERROR);

    req->method = method;

    RETURN_ERROR(HTTP_SUCCESS_ERROR);
}

int http_request_set_start_line(http_request_t *req, const char *path)
{
    HTTP_ROBUSTNESS_CHECK((req && path), HTTP_NULL_VALUE_ERROR);
    
    const char *m = _http_method_mapping[req->method];
    
    http_message_buffer_concat(req->req_msg.line, m, path, " ", _http_request_version, HTTP_CRLF, NULL);

    RETURN_ERROR(HTTP_SUCCESS_ERROR);
}

int http_request_set_start_line_with_query(http_request_t *req, const char *path, const char *query)
{
    HTTP_ROBUSTNESS_CHECK((req && path && query), HTTP_NULL_VALUE_ERROR);
    
    const char *m = _http_method_mapping[req->method];
    
    http_message_buffer_concat(req->req_msg.line, m, path, "?", query, " ", _http_request_version, HTTP_CRLF, NULL);

    RETURN_ERROR(HTTP_SUCCESS_ERROR);
}

/* adds a header to the request with given key and value. */
void http_request_add_header(http_request_t *req, const char *key, const char *value)
{
    http_message_buffer_concat(req->req_msg.header, key, ": ", value, HTTP_CRLF, NULL);
}

/* adds a header to the request with given header index and value. */
void http_request_add_header_form_index(http_request_t *req, http_request_header_t index, const char *value)
{
    HTTP_ROBUSTNESS_CHECK((req && index && value), HTTP_VOID);

    /* request header field already exists... */
    if (req->header_index & (0x01 << index))
        return;
    
    req->header_index |= (0x01 << index);
    
    const char *key = _http_request_headers_mapping[index];

    http_message_buffer_concat(req->req_msg.header, key, value, HTTP_CRLF, NULL);
}

char *http_request_get_header(http_request_t *req, const char *key)
{
    HTTP_ROBUSTNESS_CHECK((req && key), NULL);
    
    int offset = strlen(key);
    char *addr = strstr(req->req_msg.header->data, key);

    if (addr)
        return (addr + offset);
    
    return NULL;
}

char *http_request_get_header_form_index(http_request_t *req, http_request_header_t index)
{
    HTTP_ROBUSTNESS_CHECK((req && index), NULL);

    /* request header field already exists... */
    if (!(req->header_index & (0x01 << index)))
        return NULL;
    
    const char *key = _http_request_headers_mapping[index];
    
    return http_request_get_header(req, key);
}


int http_request_set_body(http_request_t *req, const char *buf, size_t size)
{
    HTTP_ROBUSTNESS_CHECK((req && buf && size), HTTP_NULL_VALUE_ERROR);

    http_message_buffer_concat(req->req_msg.body, buf, HTTP_CRLF, NULL);

    char *str = http_utils_itoa(req->req_msg.body->used, NULL, 10);
    http_request_add_header_form_index(req, HTTP_REQUEST_HEADER_CONTENT_LENGTH, str);
    http_utils_release_string(str);

    RETURN_ERROR(HTTP_SUCCESS_ERROR);
}

int http_request_set_body_form_pointer(http_request_t *req, const char *buf, size_t size)
{
    HTTP_ROBUSTNESS_CHECK((req && buf && size), HTTP_NULL_VALUE_ERROR);

    http_message_buffer_pointer(req->req_msg.body, buf, size);

    char *str = http_utils_itoa(req->req_msg.body->used, NULL, 10);
    http_request_add_header_form_index(req, HTTP_REQUEST_HEADER_CONTENT_LENGTH, str);
    http_utils_release_string(str);

    RETURN_ERROR(HTTP_SUCCESS_ERROR);
}

char *http_request_get_start_line_data(http_request_t *req)
{
    HTTP_ROBUSTNESS_CHECK(req, NULL);
    return req->req_msg.line->data;
}

char *http_request_get_header_data(http_request_t *req)
{
    HTTP_ROBUSTNESS_CHECK(req, NULL);
    return req->req_msg.header->data;
}

char *http_request_get_body_data(http_request_t *req)
{
    HTTP_ROBUSTNESS_CHECK(req, NULL);
    return req->req_msg.body->data;
}

size_t http_request_get_start_line_length(http_request_t *req)
{
    HTTP_ROBUSTNESS_CHECK(req, 0);
    return req->req_msg.line->length;
}

size_t http_request_get_header_length(http_request_t *req)
{
    HTTP_ROBUSTNESS_CHECK(req, 0);
    return req->req_msg.header->length;
}

size_t http_request_get_body_length(http_request_t *req)
{
    HTTP_ROBUSTNESS_CHECK(req, 0);
    return req->req_msg.body->length;
}

void http_request_print_start_line(http_request_t *req)
{
    HTTP_ROBUSTNESS_CHECK(req, HTTP_VOID);

    HTTP_LOG_I("%s:%d %s()...\n\n%s", __FILE__, __LINE__, __FUNCTION__, req->req_msg.line->data);
}

void http_request_print_header(http_request_t *req)
{
    HTTP_ROBUSTNESS_CHECK(req, HTTP_VOID);

    HTTP_LOG_I("%s:%d %s()...\n\n%s", __FILE__, __LINE__, __FUNCTION__, req->req_msg.header->data);
}

void http_request_print_body(http_request_t *req)
{
    HTTP_ROBUSTNESS_CHECK(req, HTTP_VOID);

    HTTP_LOG_I("%s:%d %s()...\n\n%s", __FILE__, __LINE__, __FUNCTION__, req->req_msg.body->data);
}

