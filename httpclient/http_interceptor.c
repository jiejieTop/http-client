/*
 * @Author: jiejie
 * @Github: https://github.com/jiejieTop
 * @Date: 2020-04-16 20:31:12
 * @LastEditTime: 2020-05-26 19:24:47
 * @Description: the code belongs to jiejie, please keep the author information and source code according to the license.
 */

#include <http_interceptor.h>
#include <http_log.h>
#include <http_error.h>
#include <http_utils.h>
#include <http_parser.h>
#include <http_url_parser.h>
#include <platform_memory.h>
#include <platform_timer.h>

static const char *_http_interceptor_ca = NULL;

static int _http_read_buffer(http_interceptor_t *interceptor, size_t length)
{
    int len = 0;
    platform_timer_t timer;

    HTTP_ROBUSTNESS_CHECK(interceptor, HTTP_NULL_VALUE_ERROR);

    if ((0 == length) || (length > interceptor->buffer_len))
        length = interceptor->buffer_len;
    
    memset(interceptor->buffer, 0, interceptor->buffer_len);

    platform_timer_init(&timer);
    platform_timer_cutdown(&timer, interceptor->cmd_timeout);

    len = network_read(interceptor->network, interceptor->buffer, length, platform_timer_remain(&timer));

    RETURN_ERROR(len);
}

static int _http_write_buffer(http_interceptor_t *interceptor, unsigned char *buf, int length)
{
    int len = 0;
    int sent = 0;
    platform_timer_t timer;

    platform_timer_init(&timer);
    platform_timer_cutdown(&timer, interceptor->cmd_timeout);

    /* send http buffer in a blocking manner or exit when it timer is expired */
    while ((sent < length) && (!platform_timer_is_expired(&timer))) {
        len = network_write(interceptor->network, buf, length, platform_timer_remain(&timer));
        if (len <= 0)  // there was an error writing the data
            break;
        sent += len;
    }

    if (sent == length) {
        RETURN_ERROR(HTTP_SUCCESS_ERROR);
    }
    
    RETURN_ERROR(HTTP_SEND_MESSAGE_ERROR);
}

static void _http_interceptor_set_status(http_interceptor_t *interceptor, http_interceptor_status_t status)
{
    interceptor->status = status;
}

static http_interceptor_status_t _http_interceptor_get_status(http_interceptor_t *interceptor)
{
    return interceptor->status;
}

extern const char *ca_get();

static int _http_interceptor_set_network(http_interceptor_t *interceptor)
{
    int res = HTTP_SUCCESS_ERROR;

    HTTP_ROBUSTNESS_CHECK(interceptor, HTTP_NULL_VALUE_ERROR);

    /* network init */
    if (NULL == interceptor->network) {
        interceptor->network = (network_t*) platform_memory_alloc(sizeof(network_t));
        HTTP_ROBUSTNESS_CHECK(interceptor->network, HTTP_MEM_NOT_ENOUGH_ERROR);
        memset(interceptor->network, 0, sizeof(network_t));
    }

    res = network_init( interceptor->network,
                        http_get_connect_params_host(interceptor->connect_params),
                        http_get_connect_params_port(interceptor->connect_params),
                        NULL);
    
    if (http_utils_ignore_case_match(http_get_connect_params_scheme(interceptor->connect_params), "https") == 0) {
        network_set_ca(interceptor->network, _http_interceptor_ca);
    }

    if (HTTP_SUCCESS_ERROR == res) {
        _http_interceptor_set_status(interceptor, http_interceptor_status_init);
    } else {
        _http_interceptor_set_status(interceptor, http_interceptor_status_release);
    }

    RETURN_ERROR(res);
}

static int _http_on_url(http_parser *parser, const char *at, size_t length)
{
    // HTTP_LOG_D("_http_on_url");
    return 0;
}

static int _http_on_status(http_parser *parser, const char *at, size_t length)
{
    // HTTP_LOG_D("_http_on_status：%d\n", parser->status_code);
    return 0;
}

static int _http_on_header_field(http_parser *parser, const char *at, size_t length)
{
    http_interceptor_t *interceptor = parser->data;
    // printf("%.*s :", (int)length, at);

    if(0 == http_utils_ignore_case_nmatch(at, "Location", 8)) {
        interceptor->flag.flag_t.redirects = 1;
    } else if(0 == http_utils_ignore_case_nmatch(at, "Transfer-Encoding", 17)) {
        interceptor->flag.flag_t.chunked = 1;
    } else if(0 == http_utils_ignore_case_nmatch(at, "WWW-Authenticate", 16)) {
        interceptor->flag.flag_t.authenticate = 1;
    }
    
    return 0;
}

static int _http_on_header_value(http_parser *parser, const char *at, size_t length)
{
    http_interceptor_t *interceptor = parser->data;

    if (0 != interceptor->flag.flag_t.redirects) {
        http_set_connect_params_url(interceptor->connect_params, at, length);
        interceptor->flag.flag_t.redirects = 0;
    }

    // printf("%.*s\n", (int)length, at);
    return 0;
}

static int _http_on_headers_complete(http_parser *parser)
{
    http_interceptor_t *interceptor = parser->data;

    http_response_set_status(&interceptor->response, parser->status_code);
    http_response_set_offset(&interceptor->response, parser->nread);
    http_response_set_length(&interceptor->response, parser->content_length);

    // HTTP_LOG_I("_http_on_headers_complete, status=%d, offset=%d, nread=%d, content_length = %d\n", 
    //             parser->status_code, interceptor->response.offset, parser->nread, parser->content_length);

    http_event_dispatch(interceptor->evetn, http_event_type_on_headers, interceptor, NULL, 0);

    _http_interceptor_set_status(interceptor, http_interceptor_status_headers_complete);
    return 0;
}

static int _http_on_message_begin(http_parser *parser)
{
    http_interceptor_t *interceptor = parser->data;
    HTTP_LOG_D("_http_on_message_begin");
    http_event_dispatch(interceptor->evetn, http_event_type_on_response, interceptor, NULL, 0);
    return 0;
}

static int _http_on_message_complete(http_parser *parser)
{
    http_interceptor_t *interceptor = parser->data;

    HTTP_LOG_D("_http_on_message_complete");

    if (0!= interceptor->flag.flag_t.chunked)
        interceptor->flag.flag_t.chunked_complete = 1;

    interceptor->flag.flag_t.complete = 1;      /* complete */

    return 0;
}

static int _http_on_body(http_parser *parser, const char *at, size_t length)
{
    http_interceptor_t *interceptor = parser->data;

    // HTTP_LOG_W("data_process len: %d\n", (int)length);

    interceptor->data_process += length;

    http_event_dispatch(interceptor->evetn, http_event_type_on_body, interceptor, (void *)at, length);
    return 0;
}

static int _http_on_chunk_header(http_parser *parser)
{
    // HTTP_LOG_D("_http_on_chunk_header");
    return 0;
}

static int _http_on_chunk_complete(http_parser *parser)
{
    // HTTP_LOG_D("_http_on_chunk_complete");
    return 0;
}

static int _http_interceptor_parser_setting(http_interceptor_t *interceptor)
{
    interceptor->parser_settings->on_url = _http_on_url;
    interceptor->parser_settings->on_status = _http_on_status;
    interceptor->parser_settings->on_header_field = _http_on_header_field;
    interceptor->parser_settings->on_header_value = _http_on_header_value;
    interceptor->parser_settings->on_headers_complete = _http_on_headers_complete;
    interceptor->parser_settings->on_message_begin = _http_on_message_begin;
    interceptor->parser_settings->on_message_complete = _http_on_message_complete;
    interceptor->parser_settings->on_body = _http_on_body;
    interceptor->parser_settings->on_chunk_header = _http_on_chunk_header;
    interceptor->parser_settings->on_chunk_complete = _http_on_chunk_complete;
    interceptor->parser->data = interceptor;
}

static int _http_interceptor_prepare(http_interceptor_t *interceptor)
{
    HTTP_ROBUSTNESS_CHECK(interceptor, HTTP_NULL_VALUE_ERROR);

    if (NULL == interceptor->parser)
        interceptor->parser = platform_memory_alloc(sizeof(struct http_parser));

    if (NULL == interceptor->parser_settings)
        interceptor->parser_settings = platform_memory_alloc(sizeof(struct http_parser_settings));
    
    HTTP_ROBUSTNESS_CHECK((interceptor->parser && interceptor->parser_settings), HTTP_MEM_NOT_ENOUGH_ERROR);

    http_parser_init(interceptor->parser, HTTP_RESPONSE);
    _http_interceptor_parser_setting(interceptor);

    RETURN_ERROR(HTTP_SUCCESS_ERROR);
}

int http_interceptor_check_response(http_interceptor_t *interceptor)
{
    http_response_status_t status;
    status = http_response_get_status(&interceptor->response);

    if (http_interceptor_status_headers_complete != _http_interceptor_get_status(interceptor))
        RETURN_ERROR(HTTP_SUCCESS_ERROR);

    if (0 != interceptor->flag.flag_t.complete) {
        interceptor->flag.flag_t.complete = 0;
        _http_interceptor_set_status(interceptor, http_interceptor_status_response_complete);
    }

    switch (status) {
        case http_response_status_moved_permanently:
        case http_response_status_found:
        case http_response_status_see_ther:
        case http_response_status_not_modified:
        case http_response_status_use_proxy:
        case http_response_status_unused:
        case http_response_status_temporary_redirect:
            interceptor->flag.flag_t.again = 1;
            _http_interceptor_set_status(interceptor, http_interceptor_status_release);
            http_url_parsing(interceptor->connect_params, http_get_connect_params_url(interceptor->connect_params));
            break;

        default:
            break;
    }
    RETURN_ERROR(HTTP_SUCCESS_ERROR);
}

int http_interceptor_init(http_interceptor_t *interceptor)
{
    int res = HTTP_SUCCESS_ERROR;
    HTTP_ROBUSTNESS_CHECK(interceptor, HTTP_NULL_VALUE_ERROR);

    if (http_interceptor_status_invalid != _http_interceptor_get_status(interceptor))
        RETURN_ERROR(HTTP_SUCCESS_ERROR);

    if (NULL == interceptor->network)
        interceptor->network = (network_t*) platform_memory_alloc(sizeof(network_t));
    
    HTTP_ROBUSTNESS_CHECK(interceptor->network, HTTP_MEM_NOT_ENOUGH_ERROR);

    http_request_init(&interceptor->request);
    http_response_init(&interceptor->response);

    interceptor->cmd_timeout = HTTP_DEFAULT_CMD_TIMEOUT;
    interceptor->buffer_len = HTTP_DEFAULT_BUF_SIZE;
    interceptor->data_process = 0;
    interceptor->flag.all_flag = 0;
    interceptor->owner = NULL;
    
    if (NULL == interceptor->evetn)
        interceptor->evetn = http_event_init();
    
    if (NULL == interceptor->message)
        interceptor->message = http_message_buffer_init(0);

    if (NULL == interceptor->buffer)
        interceptor->buffer = platform_memory_alloc(interceptor->buffer_len);

    HTTP_ROBUSTNESS_CHECK((interceptor->evetn && interceptor->message && interceptor->buffer), HTTP_MEM_NOT_ENOUGH_ERROR);

    res = _http_interceptor_prepare(interceptor);
    if (HTTP_SUCCESS_ERROR != res)
        RETURN_ERROR(res);
    
    RETURN_ERROR(HTTP_SUCCESS_ERROR);
}


void http_interceptor_set_ca(const char *ca)
{
    _http_interceptor_ca = ca;
}

void http_interceptor_set_owner(http_interceptor_t *interceptor, void *owner)
{
    HTTP_ROBUSTNESS_CHECK(interceptor, HTTP_VOID);
    if (http_interceptor_status_invalid != _http_interceptor_get_status(interceptor))
        return;

    interceptor->owner = owner;
}

void http_interceptor_event_register(http_interceptor_t *interceptor, http_event_cb_t cb)
{
    HTTP_ROBUSTNESS_CHECK(interceptor, HTTP_VOID);

    if (http_interceptor_status_invalid != _http_interceptor_get_status(interceptor))
        return;

    if (NULL == interceptor->evetn)
        interceptor->evetn = http_event_init();

    if (NULL != cb) {
        http_event_register(interceptor->evetn, cb);
    }
}

int http_interceptor_set_connect_params(http_interceptor_t *interceptor, http_connect_params_t *conn_param)
{
    HTTP_ROBUSTNESS_CHECK((interceptor && conn_param), HTTP_NULL_VALUE_ERROR);

    if (http_interceptor_status_invalid != _http_interceptor_get_status(interceptor))
        RETURN_ERROR(HTTP_SUCCESS_ERROR);


    interceptor->connect_params = conn_param;
    
    return _http_interceptor_set_network(interceptor);
}

int http_interceptor_connect(http_interceptor_t *interceptor)
{
    int res = HTTP_SUCCESS_ERROR;

    HTTP_ROBUSTNESS_CHECK(interceptor, HTTP_NULL_VALUE_ERROR);
    
    if (http_interceptor_status_init != _http_interceptor_get_status(interceptor))
        RETURN_ERROR(HTTP_SUCCESS_ERROR);

    res = network_connect(interceptor->network);

    if (HTTP_SUCCESS_ERROR == res) {
        _http_interceptor_set_status(interceptor, http_interceptor_status_connect);
        HTTP_LOG_D("interceptor connect success ...\n");
    } else {
        HTTP_LOG_D("interceptor connect fail ...\n");
        _http_interceptor_set_status(interceptor, http_interceptor_status_release);
    }

    RETURN_ERROR(res);
}

int http_interceptor_request(http_interceptor_t *interceptor, http_request_method_t mothod, const char *post_buf)
{
    int res = HTTP_SUCCESS_ERROR;
    HTTP_ROBUSTNESS_CHECK((interceptor && mothod), HTTP_NULL_VALUE_ERROR);
    
    if (http_interceptor_status_connect != _http_interceptor_get_status(interceptor))
        RETURN_ERROR(HTTP_SUCCESS_ERROR);

    http_request_set_method(&interceptor->request, mothod);

    if (NULL == http_get_connect_params_query(interceptor->connect_params)) {
        http_request_set_start_line(&interceptor->request, http_get_connect_params_path(interceptor->connect_params));
    } else {
        http_request_set_start_line_with_query(&interceptor->request, 
                                                http_get_connect_params_path(interceptor->connect_params), 
                                                http_get_connect_params_query(interceptor->connect_params));
    }

    http_request_header_init(&interceptor->request);
    http_request_add_header_form_index(&interceptor->request, 
                                        HTTP_REQUEST_HEADER_HOST, 
                                        http_get_connect_params_host(interceptor->connect_params));
    
    if (NULL != post_buf) {

        int len = strlen(post_buf);
        
        /* don't use too much memory */
        if (len > HTTP_DEFAULT_BUF_SIZE) {
            http_request_set_body_form_pointer(&interceptor->request, post_buf, len);
        } else {
            http_request_set_body(&interceptor->request, post_buf, len);
        }

        http_request_add_header_form_index(&interceptor->request, 
                                            HTTP_REQUEST_HEADER_CONTENT_TYPE, 
                                            "text/plain");
    }

    http_message_buffer_reinit(interceptor->message);

    http_message_buffer_cover(interceptor->message, 
                              http_request_get_start_line_data(&interceptor->request), 
                              http_request_get_header_data(&interceptor->request), 
                              HTTP_CRLF, NULL);                  

    http_message_buffer_concat( interceptor->message, 
                                http_request_get_body_data(&interceptor->request), 
                                HTTP_CRLF, NULL);

    // HTTP_LOG_D("len:%ld\ndata:%s",interceptor->write_buf->used, interceptor->write_buf->data);
    res = _http_write_buffer(interceptor, 
                            http_message_buffer_get_data(interceptor->message), 
                            http_message_buffer_get_used(interceptor->message));
    
    http_event_dispatch(interceptor->evetn, 
                        http_event_type_on_request, 
                        interceptor, 
                        http_message_buffer_get_data(interceptor->message), 
                        http_message_buffer_get_used(interceptor->message));

    if (HTTP_SUCCESS_ERROR == res) {

        /* send data successfully, free the memory space of the request message, emm... no keep alive */
        http_request_release(&interceptor->request);

        _http_interceptor_set_status(interceptor, http_interceptor_status_request);

        if (interceptor->message) {
            http_message_buffer_release(interceptor->message);
            interceptor->message = NULL;
        }

    } else {
        _http_interceptor_set_status(interceptor, http_interceptor_status_release);
    }
    
    RETURN_ERROR(res);
}

int http_interceptor_fetch_headers(http_interceptor_t *interceptor)
{
    int len = 0;

    HTTP_ROBUSTNESS_CHECK(interceptor, 0);
    
    if (http_interceptor_status_request != _http_interceptor_get_status(interceptor))
        RETURN_ERROR(HTTP_SUCCESS_ERROR);

    _http_interceptor_set_status(interceptor, http_interceptor_status_response_headers);

    while (http_interceptor_status_response_headers == _http_interceptor_get_status(interceptor)) {
        
        len = _http_read_buffer(interceptor, interceptor->buffer_len);

        if (len <= 0) {
            return -1;
        } else {
            http_parser_execute(interceptor->parser, interceptor->parser_settings, interceptor->buffer, len);
        }
    }

    RETURN_ERROR(len);
}

int http_interceptor_submit_data(http_interceptor_t *interceptor)
{
    // HTTP_LOG_I("http_interceptor_submit_data:\n%s\n", http_response_get_message_body(&interceptor->response));

    if ((http_interceptor_status_response_complete != _http_interceptor_get_status(interceptor)) || 
        (http_interceptor_status_response_body != _http_interceptor_get_status(interceptor)))
        RETURN_ERROR(HTTP_SUCCESS_ERROR);
    
    http_event_dispatch(interceptor->evetn, 
                        http_event_type_on_submit, 
                        interceptor, 
                        http_response_get_message_data(&interceptor->response), 
                        http_response_get_message_len(&interceptor->response));

    _http_interceptor_set_status(interceptor, http_interceptor_status_release);
}

int http_interceptor_release(http_interceptor_t *interceptor)
{
    HTTP_ROBUSTNESS_CHECK(interceptor, HTTP_NULL_VALUE_ERROR);
    
    if (interceptor->network) {
        network_release(interceptor->network);
        platform_memory_free(interceptor->network);
        interceptor->network = NULL;
    }

    if (interceptor->parser) {
        platform_memory_free(interceptor->parser);
        interceptor->parser = NULL;
    }
  
    if (interceptor->parser_settings) {
        platform_memory_free(interceptor->parser_settings);
        interceptor->parser_settings = NULL;
    }

    if (interceptor->message) {
        http_message_buffer_release(interceptor->message);
        interceptor->message = NULL;
    }

    if (interceptor->evetn) {
        http_event_release(interceptor->evetn);
        interceptor->evetn = NULL;
    }

    if (interceptor->buffer) {
        platform_memory_free(interceptor->buffer);
        interceptor->buffer = NULL;
    }

    http_request_release(&interceptor->request);
    http_response_release(&interceptor->response);

    _http_interceptor_set_status(interceptor, http_interceptor_status_invalid);

    RETURN_ERROR(HTTP_SUCCESS_ERROR);
}

int http_interceptor_fetch_data(http_interceptor_t *interceptor)
{
    int len = 0;
    HTTP_ROBUSTNESS_CHECK(interceptor, len);
    
    if (http_interceptor_status_headers_complete != _http_interceptor_get_status(interceptor))
        RETURN_ERROR(len);

    do {
        len = _http_read_buffer(interceptor, (http_response_get_length(&interceptor->response) - interceptor->data_process));
        
        if (len <= 0) {
            return -1;
        } else {
            http_parser_execute(interceptor->parser, interceptor->parser_settings, interceptor->buffer, len);
        }
    } while ((interceptor->data_process < interceptor->response.length) ||
            ((interceptor->flag.flag_t.chunked && !interceptor->flag.flag_t.chunked_complete)));

    RETURN_ERROR(len);
}


int http_interceptor_process(http_interceptor_t *interceptor,
                             http_connect_params_t *connect_params,
                             http_request_method_t mothod, 
                             void *post_buf,
                             void *owner,
                             http_event_cb_t cb)
{
    int res = HTTP_SUCCESS_ERROR;
    HTTP_ROBUSTNESS_CHECK((interceptor && connect_params && mothod), HTTP_NULL_VALUE_ERROR);

    do {
        if (interceptor->flag.flag_t.again) {
            interceptor->flag.flag_t.again = 0;
            interceptor->status = 0;
        }

        switch (interceptor->status) {
            case http_interceptor_status_invalid:
                http_interceptor_init(interceptor);
                http_interceptor_set_owner(interceptor, owner);
                http_interceptor_event_register(interceptor, cb);
                http_interceptor_set_connect_params(interceptor, connect_params);
                
            case http_interceptor_status_init:
                http_interceptor_connect(interceptor);

            case http_interceptor_status_connect:
                http_interceptor_request(interceptor, mothod, post_buf);

            case http_interceptor_status_request:
                http_interceptor_fetch_headers(interceptor);

            case http_interceptor_status_headers_complete:
                http_interceptor_check_response(interceptor);
                http_interceptor_fetch_data(interceptor);
            
            case http_interceptor_status_response_body:
            case http_interceptor_status_response_complete:
                http_interceptor_submit_data(interceptor);

            case http_interceptor_status_release:
                http_interceptor_release(interceptor);

            default:
                break;
        }

    } while (interceptor->flag.flag_t.again);

}



