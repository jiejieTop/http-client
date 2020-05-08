/*
 * @Author: jiejie
 * @Github: https://github.com/jiejieTop
 * @Date: 2020-04-16 20:31:12
 * @LastEditTime: 2020-05-08 22:39:24
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


// static int _http_read_packet(http_interceptor_t *interceptor, platform_timer_t* timer)
// {
//     HTTP_ROBUSTNESS_CHECK((interceptor && timer), HTTP_NULL_VALUE_ERROR);

//     platform_timer_init(timer);
//     platform_timer_cutdown(timer, interceptor->cmd_timeout);

//     /* 1. read the header byte.  This has the packet type in it */
//     rc = network_read(c->network, c->read_buf, len, platform_timer_remain(timer));
//     if (rc != len)
//         RETURN_ERROR(HTTP_NOTHING_TO_READ_ERROR);

//     /* 2. read the remaining length.  This is variable in itself */
//     mqtt_decode_packet(c, &remain_len, platform_timer_remain(timer));

//     /* put the original remaining length back into the buffer */
//     len += MQTTPacket_encode(c->read_buf + len, remain_len); 

//     if ((len + remain_len) > c->read_buf_size) {
        
//         /* mqtt buffer is too short, read and discard all corrupted data */
//         mqtt_packet_drain(c, timer, remain_len);

//         RETURN_ERROR(HTTP_BUFFER_TOO_SHORT_ERROR);
//     }

//     /* 3. read the rest of the buffer using a callback to supply the rest of the data */
//     if ((remain_len > 0) && ((rc = c->network->read(c->network, c->read_buf + len, remain_len, platform_timer_remain(timer))) != remain_len))
//         RETURN_ERROR(HTTP_NOTHING_TO_READ_ERROR);

//     header.byte = c->read_buf[0];
//     *packet_type = header.bits.type;
    
//     platform_timer_cutdown(&c->last_received, (c->connect_params->keep_alive_interval * 1000)); 

//     RETURN_ERROR(HTTP_SUCCESS_ERROR);
// }

static int _http_send_buffer(http_interceptor_t *interceptor, unsigned char *buf, int length)
{
    int len = 0;
    int sent = 0;
    platform_timer_t timer;

    platform_timer_init(&timer);
    platform_timer_cutdown(&timer, interceptor->cmd_timeout);

    /* send mqtt packet in a blocking manner or exit when it timer is expired */
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

    network_set_addr_port(interceptor->network, interceptor->connect_params->http_host, interceptor->connect_params->http_port);

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

    interceptor->cmd_timeout = 5000;

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

    res =  _http_interceptor_prepare(interceptor);
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



int http_interceptor_request(http_interceptor_t *interceptor, http_request_method_t mothod)
{
    HTTP_ROBUSTNESS_CHECK((interceptor && mothod), HTTP_NULL_VALUE_ERROR);
    
    http_request_init(&interceptor->request);
    http_request_set_method(&interceptor->request, mothod);

    if (NULL == interceptor->connect_params->http_query) {
        http_request_set_start_line(&interceptor->request, interceptor->connect_params->http_path);
    } else {
        http_request_set_start_line_with_query(&interceptor->request, interceptor->connect_params->http_path, interceptor->connect_params->http_query);
    }

    http_request_header_init(&interceptor->request);
    http_request_add_header_form_index(&interceptor->request, HTTP_REQUEST_HEADER_HOST, http_get_connect_params_host(interceptor->connect_params));
    
    http_request_set_body(&interceptor->request, interceptor->connect_params->http_url, strlen(interceptor->connect_params->http_url));

    // http_request_print_start_line(&interceptor->request);
    // http_request_print_header(&interceptor->request);
    // http_request_print_body(&interceptor->request);

    _http_send_buffer(interceptor, http_request_get_start_line_data(&interceptor->request), http_request_get_start_line_length(&interceptor->request));
    _http_send_buffer(interceptor, http_request_get_header_data(&interceptor->request), http_request_get_header_length(&interceptor->request));
    // _http_send_buffer(interceptor, http_request_get_start_line_data(&interceptor->request), http_request_get_start_line_length(&interceptor->request));
    
}



