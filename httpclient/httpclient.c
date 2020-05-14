/*
 * @Author: jiejie
 * @Github: https://github.com/jiejieTop
 * @Date: 2019-12-09 21:31:25
 * @LastEditTime: 2020-05-14 10:04:30
 * @Description: the code belongs to jiejie, please keep the author information and source code according to the license.
 */
#include "httpclient.h"
#include <http_log.h>
#include <http_error.h>
#include <http_list.h>
#include <platform_memory.h>

#ifndef HTTP_CLIENT_POOL_SIZE
    #define HTTP_CLIENT_POOL_SIZE   5
#endif // !HTTP_CLIENT_POOL_SIZE

#if HTTP_CLIENT_POOL_SIZE > 32U
    #define     HTTP_CLIENT_INDEX_TAB   (((HTTP_CLIENT_POOL_SIZE -1 )/32) + 1)
    #define     HTTP_CLIENT_INDEX(i)    (((i)/32))
    static uint32_t _http_client_index[HTTP_CLIENT_INDEX_TAB];
    #define     HTTP_CLIENT_SET_FREE(i) { if (i < (HTTP_CLIENT_POOL_SIZE - 1)) { (_http_client_index[HTTP_CLIENT_INDEX(i)]) |= (0x01 << ((i) % 32)); } }
    #define     HTTP_CLIENT_SET_USER(i) { if (i < (HTTP_CLIENT_POOL_SIZE - 1)) { (_http_client_index[HTTP_CLIENT_INDEX(i)]) &= ~(0x01 << ((i) % 32)); } }
#else
    static uint32_t _http_client_index;
    #define     HTTP_CLIENT_SET_FREE(i) { if (i < (HTTP_CLIENT_POOL_SIZE - 1)) { (_http_client_index) |= (0x01 << (i)); } }
    #define     HTTP_CLIENT_SET_USER(i) { if (i < (HTTP_CLIENT_POOL_SIZE - 1)) { (_http_client_index) &= ~(0x01 << (i)); } }
#endif

static http_client_t _http_client_pool[HTTP_CLIENT_POOL_SIZE] = {0};

static const uint8_t _http_client_pool_bitmap[] = {
    0, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,    /* 00 */
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,    /* 10 */
    5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,    /* 20 */
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,    /* 30 */
    6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,    /* 40 */
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,    /* 50 */
    5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,    /* 60 */
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,    /* 70 */
    7, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,    /* 80 */
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,    /* 90 */
    5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,    /* A0 */
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,    /* B0 */
    6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,    /* C0 */
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,    /* D0 */
    5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,    /* E0 */
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0     /* F0 */
};

/**
 * get Highest priority by bit map
 */
static uint32_t _http_client_get_index_form_bitmap(uint32_t index)
{
    if (index == 0) return 0;
    
    if (index & 0xff)
        return _http_client_pool_bitmap[index & 0xff];

    if (index & 0xff00)
        return _http_client_pool_bitmap[(index & 0xff00) >> 8] + 8;

    if (index & 0xff0000)
        return _http_client_pool_bitmap[(index & 0xff0000) >> 16] + 16;

    return _http_client_pool_bitmap[(index & 0xff000000) >> 24] + 24;
}

static uint32_t _http_client_get_index(void)
{
    uint32_t index;
#if HTTP_CLIENT_POOL_SIZE > 32
    uint32_t i;
    for (i = 0; i < HTTP_CLIENT_INDEX_TAB; i++) {
        if (_http_client_index[i] & 0xFFFFFFFF)
            break;
    }

    index = _http_client_get_index_form_bitmap(_http_client_index[i]) + (32 * i);
#else
    index = _http_client_get_index_form_bitmap(_http_client_index);
#endif
    return (index >= HTTP_CLIENT_POOL_SIZE) ? HTTP_CLIENT_POOL_SIZE : index;
}


http_connect_status_t http_get_connect_status(http_client_t *c)
{
    return c->status;
}

void http_set_connect_status(http_client_t *c, http_connect_status_t state)
{
    platform_mutex_lock(&c->global_lock);
    c->status = state;
    platform_mutex_unlock(&c->global_lock);
}

int _http_client_init(http_client_t *c)
{
    HTTP_ROBUSTNESS_CHECK(c, HTTP_NULL_VALUE_ERROR);

    memset(c, 0, sizeof(http_client_t));

    http_list_init(&c->connect_pool_list);

    if (NULL == c->connect_params) {
        c->connect_params = http_assign_connect_params();
    }

    if (NULL == c->interceptor) {
        c->interceptor = platform_memory_alloc(sizeof(http_interceptor_t));
        memset(c->interceptor, 0, sizeof(http_interceptor_t));
    }

    platform_mutex_init(&c->global_lock);
}

void *_http_client_pool_init(void)
{
    int i;

#if HTTP_CLIENT_POOL_SIZE > 32
    for (i = 0; i < HTTP_CLIENT_INDEX_TAB; i++)
        _http_client_index[i] = 0xFFFFFFFF;
#else
    _http_client_index = 0xFFFFFFFF;
#endif

    for (i = 0; i < HTTP_CLIENT_POOL_SIZE; i++) {
        _http_client_init(&_http_client_pool[i]);
    }
}


http_client_t *http_client_assign(void)
{
    
}

int http_client_get_data(const char *url)
{
    int i;
    uint32_t index;

    HTTP_ROBUSTNESS_CHECK(url, HTTP_NULL_VALUE_ERROR);

    _http_client_pool_init();

    for (i = 0; i < HTTP_CLIENT_POOL_SIZE; i++) {
        
        index = _http_client_get_index();
        HTTP_CLIENT_SET_USER(index);
        printf("user [%d]:index = %d\n", i, index);
        
        // HTTP_CLIENT_SET_FREE(i);

        // index = _http_client_get_index();

        // printf("free [%d]:index = %d\n", i, index);
        // HTTP_CLIENT_SET_USER(i);
    }

}



