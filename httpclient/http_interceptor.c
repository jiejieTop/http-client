/*
 * @Author: jiejie
 * @Github: https://github.com/jiejieTop
 * @Date: 2020-04-16 20:31:12
 * @LastEditTime: 2020-05-09 21:55:13
 * @Description: the code belongs to jiejie, please keep the author information and source code according to the license.
 */

#include <http_interceptor.h>
#include <http_log.h>
#include <http_error.h>
#include <http_utils.h>
#include <http_parser.h>
#include <http_url_parser.h>
#include <http_connector.h>
#include <platform_memory.h>
#include <platform_timer.h>


static int _http_read_buffer(http_interceptor_t *interceptor, http_message_buffer_t *buf, int length)
{
    int read_len = 0;

    platform_timer_t timer;
    HTTP_ROBUSTNESS_CHECK((interceptor && buf && length), HTTP_NULL_VALUE_ERROR);

    platform_timer_init(&timer);
    platform_timer_cutdown(&timer, interceptor->cmd_timeout);
    
    http_message_buffer_reinit(interceptor->message);

    do {
        memset(interceptor->message->data, 0, length);
        
        read_len = network_read(interceptor->network, 
                                interceptor->message->data, 
                                length,
                                platform_timer_remain(&timer));

        http_message_buffer_concat(buf, interceptor->message->data, NULL);

    } while (read_len > 0); 
    
    RETURN_ERROR(HTTP_SUCCESS_ERROR);
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
    
    RETURN_ERROR(HTTP_SEND_PACKET_ERROR);
}

static void _http_interceptor_set_status(http_interceptor_t *interceptor, http_interceptor_status_t status)
{
    interceptor->status = status;
}

static http_interceptor_status_t _http_interceptor_get_status(http_interceptor_t *interceptor)
{
    return interceptor->status;
}

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

    network_set_addr_port(interceptor->network, 
                          interceptor->connect_params->http_host, 
                          interceptor->connect_params->http_port);

    res = network_init(interceptor->network, &interceptor->network->network_params);
    
    if (HTTP_SUCCESS_ERROR == res) {
        _http_interceptor_set_status(interceptor, http_interceptor_status_init);
    } else {
        platform_memory_free(interceptor->network);
    }

    RETURN_ERROR(res);
}

static int _http_interceptor_prepare(http_interceptor_t *interceptor)
{
    HTTP_ROBUSTNESS_CHECK(interceptor, HTTP_NULL_VALUE_ERROR);

    if (NULL == interceptor->parser)
        interceptor->parser = platform_memory_alloc(sizeof(struct http_parser));
    
    HTTP_ROBUSTNESS_CHECK(interceptor->parser, HTTP_MEM_NOT_ENOUGH_ERROR);

    http_parser_init(interceptor->parser, HTTP_RESPONSE);

    RETURN_ERROR(HTTP_SUCCESS_ERROR);
}


int http_interceptor_init(http_interceptor_t *interceptor)
{
    HTTP_ROBUSTNESS_CHECK(interceptor, HTTP_NULL_VALUE_ERROR);

    interceptor->network = (network_t*) platform_memory_alloc(sizeof(network_t));
    interceptor->parser = (struct http_parser *)platform_memory_alloc(sizeof(struct http_parser));

    HTTP_ROBUSTNESS_CHECK((interceptor->network && interceptor->parser), HTTP_MEM_NOT_ENOUGH_ERROR);

    http_request_init(&interceptor->request);
    http_response_init(&interceptor->response);

    interceptor->cmd_timeout = HTTP_DEFAULT_CMD_TIMEOUT;
    interceptor->message_len = HTTP_DEFAULT_BUF_SIZE;

    interceptor->message = http_message_buffer_init(interceptor->message_len);
    HTTP_ROBUSTNESS_CHECK((interceptor->message), HTTP_MEM_NOT_ENOUGH_ERROR);

    RETURN_ERROR(HTTP_SUCCESS_ERROR);
}

int http_interceptor_set_connect_params(http_interceptor_t *interceptor, http_connect_params_t *conn_param)
{
    HTTP_ROBUSTNESS_CHECK((interceptor && conn_param), HTTP_NULL_VALUE_ERROR);

    interceptor->connect_params = conn_param;
}

int http_interceptor_connect(http_interceptor_t *interceptor)
{
    int res = HTTP_SUCCESS_ERROR;

    res = _http_interceptor_prepare(interceptor);
    if (HTTP_SUCCESS_ERROR != res)
        RETURN_ERROR(res);
    
    _http_interceptor_set_network(interceptor);

    res = network_connect(interceptor->network);

    if (HTTP_SUCCESS_ERROR == res) {
        _http_interceptor_set_status(interceptor, http_interceptor_status_connect);
    } else {
        network_release(interceptor->network);
    }

    RETURN_ERROR(res);
}



int http_interceptor_request(http_interceptor_t *interceptor, http_request_method_t mothod, const char *post_buf)
{
    int res = HTTP_SUCCESS_ERROR;
    HTTP_ROBUSTNESS_CHECK((interceptor && mothod), HTTP_NULL_VALUE_ERROR);
    
    http_request_set_method(&interceptor->request, mothod);

    if (NULL == interceptor->connect_params->http_query) {
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
    res = _http_write_buffer(interceptor, interceptor->message->data, interceptor->message->used);
    
    _http_read_buffer(interceptor, interceptor->response.message, HTTP_DEFAULT_BUF_SIZE);

    printf("%s\n", interceptor->response.message->data);

    if (HTTP_SUCCESS_ERROR == res)
        _http_interceptor_set_status(interceptor, http_interceptor_status_request);

    RETURN_ERROR(res);
}


int http_interceptor_process(http_interceptor_t *interceptor,
                             http_connect_params_t *connect_params,
                             http_request_method_t mothod, 
                             const char *post_buf)
{
    do {
        if (interceptor->process_again) {
            ;
        }

        switch (interceptor->status) {
            case http_interceptor_status_invalid:
                http_interceptor_init(interceptor);
                printf("1\n");
                

            case http_interceptor_status_init:
                http_interceptor_set_connect_params(interceptor, connect_params);
                http_interceptor_connect(interceptor);
                

            case http_interceptor_status_connect:
                http_interceptor_request(interceptor, mothod, post_buf);
                

            case http_interceptor_status_response_headers:
                ;
                break;

            case http_interceptor_status_response_body:
                ;
                break;

            case http_interceptor_status_close:
                ;
                break;

            default:
                break;
        }

    } while (interceptor->process_again);

}



