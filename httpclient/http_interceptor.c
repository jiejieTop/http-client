/*
 * @Author: jiejie
 * @Github: https://github.com/jiejieTop
 * @Date: 2020-04-16 20:31:12
 * @LastEditTime: 2020-05-06 09:08:40
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


static int _http_interceptor_set_network(http_interceptor_t *interceptor)
{
    int rse = HTTP_SUCCESS_ERROR;

    HTTP_ROBUSTNESS_CHECK(interceptor, HTTP_NULL_VALUE_ERROR);

    interceptor->network_params.addr = interceptor->connect_params.http_host;
    interceptor->network_params.port = http_utils_itoa(interceptor->connect_params.http_port, NULL, 10);
    
    /* network init */
    if (NULL == interceptor->network)
        interceptor->network = (network_t*) platform_memory_alloc(sizeof(network_t));

    HTTP_ROBUSTNESS_CHECK(interceptor->network, HTTP_MEM_NOT_ENOUGH_ERROR);

    memset(interceptor->network, 0, sizeof(network_t));

    rse = network_init(interceptor->network, &interceptor->network_params);
    
    RETURN_ERROR(rse);
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

    RETURN_ERROR(HTTP_SUCCESS_ERROR);
}

int http_interceptor_connect(http_interceptor_t *interceptor)
{
    int res = HTTP_SUCCESS_ERROR;

    res =  _http_interceptor_prepare(interceptor);
    if (HTTP_SUCCESS_ERROR != res)
        RETURN_ERROR(res);
    
    _http_interceptor_set_network(interceptor);

    res = interceptor->network->connect(interceptor->network);

    RETURN_ERROR(res);
}



int http_interceptor_request(http_interceptor_t *interceptor)
{

}



