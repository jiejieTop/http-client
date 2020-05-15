/*
 * @Author: jiejie
 * @Github: https://github.com/jiejieTop
 * @Date: 2019-12-09 21:31:25
 * @LastEditTime: 2020-05-15 16:13:37
 * @Description: the code belongs to jiejie, please keep the author information and source code according to the license.
 */
#include "httpclient.h"
#include <http_log.h>
#include <http_error.h>
#include <http_list.h>
#include <http_event.h>
#include <platform_memory.h>

#ifndef HTTP_CLIENT_POOL_SIZE
    #define HTTP_CLIENT_POOL_SIZE   5
#endif // !HTTP_CLIENT_POOL_SIZE

static http_list_t _http_client_free_list;
static http_list_t _http_client_used_list;

static int _http_client_internal_event_handle(void *e)
{
    http_event_t *event = e;
    http_interceptor_t *interceptor = event->context;
    http_client_t *c = (http_client_t*)interceptor->owner;

    if (0 == c->interest_event)
        RETURN_ERROR(HTTP_SUCCESS_ERROR);

    if (c->interest_event & event->type) {
            http_event_dispatch(c->event, event->type, c, event->data, event->len);
    }
    return 0;
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

int _http_client_internal_init(void)
{
    http_client_t *c = NULL;
    int len = sizeof(http_client_t);
    
    c = platform_memory_alloc(len);
    
    memset(c, 0, sizeof(http_client_t));

    http_list_init(&c->list);

    http_list_add(&c->list, &_http_client_free_list);

    if (NULL == c->connect_params) {
        c->connect_params = http_assign_connect_params();
    }

    if (NULL == c->event) {
        c->event = http_event_init();
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

    http_list_init(&_http_client_free_list);
    http_list_init(&_http_client_used_list);

    for (i = 0; i < HTTP_CLIENT_POOL_SIZE; i++) {
        _http_client_internal_init();
    }
}

int _http_client_handle(const char *url, void *data, http_request_method_t opt, http_event_cb_t cb)
{
    http_client_t *c = NULL;

    HTTP_ROBUSTNESS_CHECK(url, HTTP_NULL_VALUE_ERROR);

    c = http_client_assign();

    HTTP_ROBUSTNESS_CHECK(c, HTTP_FAILED_ERROR);

    http_client_set_interest_event(c, http_event_type_on_body);

    http_event_register(c->event, cb);

    http_url_parsing(c->connect_params, url);

    http_interceptor_process(c->interceptor, 
                             c->connect_params, 
                             opt, 
                             data, 
                             c,
                             _http_client_internal_event_handle);
    http_client_release(c);
}

int http_client_init(void)
{
    _http_client_pool_init();
}


http_client_t *http_client_assign(void)
{
    http_client_t *c = NULL;
    
    c = LIST_FIRST_ENTRY_OR_NULL(&_http_client_free_list, http_client_t, list);
    if (c != NULL) {
        http_list_del(&c->list);
        http_list_add(&c->list, &_http_client_used_list);
    }

    return c;
}

void http_client_release(http_client_t *c)
{
    HTTP_ROBUSTNESS_CHECK(c, HTTP_VOID);

    http_list_del(&c->list);
    http_list_add(&c->list, &_http_client_free_list);

    http_interceptor_release(c->interceptor);
    

}

void http_client_set_interest_event(http_client_t *c, http_event_type_t event)
{
    HTTP_ROBUSTNESS_CHECK((c && event), HTTP_VOID);

    c->interest_event = event;
}

int http_client_get(const char *url, http_event_cb_t cb)
{
    return _http_client_handle(url, NULL, HTTP_REQUEST_METHOD_GET, cb);
}

int http_client_post(const char *url, void *data, http_event_cb_t cb)
{
    return _http_client_handle(url, data, HTTP_REQUEST_METHOD_POST, cb);
}


