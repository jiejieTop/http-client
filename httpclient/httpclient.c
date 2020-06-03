/*
 * @Author: jiejie
 * @Github: https://github.com/jiejieTop
 * @Date: 2019-12-09 21:31:25
 * @LastEditTime: 2020-06-03 16:42:00
 * @Description: the code belongs to jiejie, please keep the author information and source code according to the license.
 */
#include "httpclient.h"
#include <http_log.h>
#include <http_error.h>
#include <http_wq.h>
#include <http_list.h>
#include <http_event.h>
#include <platform_memory.h>

static http_list_t _http_client_free_list;
static http_list_t _http_client_used_list;
static platform_mutex_t _client_pool_lock;

static void _http_client_variables_init(http_client_t *c)
{
    c->method = HTTP_REQUEST_METHOD_INVALID;
    c->interest_event = 0;
    c->process = 0;
    c->total = 0;
    c->data = NULL;
}

static int _http_client_internal_event_handle(void *e)
{
    http_event_t *event = e;
    http_interceptor_t *interceptor = event->context;
    http_client_t *c = (http_client_t*)interceptor->owner;

    c->process = interceptor->data_process;
    c->total = http_response_get_length(&interceptor->response);
    
    if (0 == c->interest_event)
        RETURN_ERROR(HTTP_SUCCESS_ERROR);

    if (c->interest_event & event->type) {
        if (c->interest_event & http_event_type_on_request) {
            http_interceptor_set_keep_alive(interceptor);
            c->interest_event &= ~http_event_type_on_request;
        } else {
            http_event_dispatch(c->event, event->type, c, event->data, event->len);
        }
    }
    return 0;
}

static void _http_client_wq_handle(void *client)
{
    http_client_t *c = (http_client_t*)client;

    http_interceptor_process(c->interceptor, 
                             c->connect_params, 
                             c->method, 
                             c->data, 
                             c,
                             _http_client_internal_event_handle);
    if (!c->flag.flag_t.keep_alive)
        http_client_release(c);
}

int _http_client_create(void)
{
    http_client_t *c = NULL;
    int len = sizeof(http_client_t);
    
    c = platform_memory_alloc(len);
    
    memset(c, 0, len);

    http_list_init(&c->list);

    http_list_add(&c->list, &_http_client_free_list);

    if (NULL == c->connect_params) {
        c->connect_params = http_assign_connect_params();
    }

    if (NULL == c->event) {
        c->event = http_event_init();
    }

    if (NULL == c->interceptor) {
        len = sizeof(http_interceptor_t);
        c->interceptor = platform_memory_alloc(len);
        memset(c->interceptor, 0, len);
    }
}

void *_http_client_pool_init(void)
{
    int i;

    platform_mutex_init(&_client_pool_lock);

    http_list_init(&_http_client_free_list);
    http_list_init(&_http_client_used_list);

    for (i = 0; i < HTTP_CLIENT_POOL_SIZE; i++) {
        _http_client_create();
    }
}

void _http_client_destroy(http_client_t *c)
{
    HTTP_ROBUSTNESS_CHECK(c, HTTP_VOID);

    if (NULL == c->connect_params) {
        http_release_connect_params(c->connect_params);
        c->connect_params = NULL;
    }

    if (NULL == c->event) {
        http_event_release(c->event);
        c->event = NULL;
    }

    if (c->interceptor) {
        platform_memory_free(c->interceptor);
        c->interceptor = NULL;
    }

    http_list_del(&c->list);
}

http_client_t *_http_client_handle(http_client_t *c, const char *url, void *data, http_request_method_t method, http_event_type_t et, http_event_cb_t cb, uint32_t flags)
{
    HTTP_ROBUSTNESS_CHECK(url, NULL);

    if (NULL == c)
        c = http_client_lease();
    
    HTTP_ROBUSTNESS_CHECK(c, NULL);

    if (flags & http_keep_alive_flag)
        c->flag.flag_t.keep_alive = 1;

    http_client_set_interest_event(c, et);
    http_client_set_method(c, method);
    http_client_set_data(c, data);
    http_event_register(c->event, cb);
    http_url_parsing(c->connect_params, url);

#ifdef HTTP_USING_WORK_QUEUE
    http_wq_add_task(_http_client_wq_handle, c, sizeof(c));
#else
    printf("\nhttp_interceptor_process start\n");
    int res = http_interceptor_process( c->interceptor, 
                                        c->connect_params, 
                                        c->method, 
                                        c->data, 
                                        c,
                                        _http_client_internal_event_handle);
    printf("\nhttp_interceptor_process end\n");
    if (!c->flag.flag_t.keep_alive)
        http_client_release(c);
    
    if (res < 0)
        return NULL;
#endif // HTTP_USING_WORK_QUEUE 
    return c;
}

int http_client_init(const char *ca)
{
    _http_client_pool_init();
    http_interceptor_set_ca(ca);

#ifdef HTTP_USING_WORK_QUEUE
    http_wq_pool_init();
#endif
    RETURN_ERROR(HTTP_SUCCESS_ERROR);
}

void http_client_exit(void)
{
    http_client_t *c = NULL;
    
#ifdef HTTP_USING_WORK_QUEUE
    http_wq_wait_exit();
    http_wq_pool_deinit();
#endif

    do {
        c = HTTP_LIST_FIRST_ENTRY_OR_NULL(&_http_client_free_list, http_client_t, list);
        if (NULL == c)
            break;

        _http_client_destroy(c);
        platform_memory_free(c);
    } while (c != NULL);

    do {
        c = HTTP_LIST_FIRST_ENTRY_OR_NULL(&_http_client_used_list, http_client_t, list);
        if (NULL == c)
            break;

        _http_client_destroy(c);
        platform_memory_free(c);
    } while (c != NULL);
}


http_client_t *http_client_lease(void)
{
    http_client_t *c = NULL;
    
    c = HTTP_LIST_FIRST_ENTRY_OR_NULL(&_http_client_free_list, http_client_t, list);
    if (c != NULL) {
        platform_mutex_lock(&_client_pool_lock);

        http_list_del(&c->list);
        http_list_add(&c->list, &_http_client_used_list);

        platform_mutex_unlock(&_client_pool_lock);
    }

    return c;
}

void http_client_release(http_client_t *c)
{
    HTTP_ROBUSTNESS_CHECK(c, HTTP_VOID);

    platform_mutex_lock(&_client_pool_lock);

    _http_client_variables_init(c);

    http_list_del(&c->list);
    http_list_add(&c->list, &_http_client_free_list);

    platform_mutex_unlock(&_client_pool_lock);

    http_interceptor_release(c->interceptor);

    http_release_connect_params_variables(c->connect_params);
}

void http_client_set_interest_event(http_client_t *c, http_event_type_t event)
{
    HTTP_ROBUSTNESS_CHECK((c && event), HTTP_VOID);

    c->interest_event = event;
}

void http_client_set_method(http_client_t *c, http_request_method_t method)
{
    HTTP_ROBUSTNESS_CHECK((c && method), HTTP_VOID);
    c->method = method;
}

void http_client_set_data(http_client_t *c, void *data)
{
    HTTP_ROBUSTNESS_CHECK(c, HTTP_VOID);
    c->data = data;
}

int http_client_get(const char *url, http_event_cb_t cb)
{
    _http_client_handle(NULL, url, NULL, HTTP_REQUEST_METHOD_GET, http_event_type_on_body, cb, 0);
    RETURN_ERROR(HTTP_SUCCESS_ERROR);
}

int http_client_post(const char *url, void *data, http_event_cb_t cb)
{
    _http_client_handle(NULL, url, data, HTTP_REQUEST_METHOD_POST, http_event_type_on_body, cb, 0);
    RETURN_ERROR(HTTP_SUCCESS_ERROR);
}


http_client_t *http_client_get_persistent(http_client_t *c, const char *url, http_event_cb_t cb)
{
    if (NULL == c){
        c = _http_client_handle(c, url, NULL, HTTP_REQUEST_METHOD_GET,(http_event_type_on_body | http_event_type_on_request), cb, http_keep_alive_flag);
    } else {
        _http_client_handle(c, url, NULL, HTTP_REQUEST_METHOD_GET, http_event_type_on_body, cb, 0);
    }
    return c;
}

