/*
 * @Author: jiejie
 * @Github: https://github.com/jiejieTop
 * @Date: 2019-12-09 21:31:25
 * @LastEditTime: 2020-06-02 22:36:15
 * @Description: the code belongs to jiejie, please keep the author information and source code according to the license.
 */
#ifndef _HTTPCLIENT_H_
#define _HTTPCLIENT_H_

#include <stdio.h>
#include <string.h>

#include "http_parser.h"
#include "http_utils.h"
#include "http_url_parser.h"
#include "http_interceptor.h"

#include "http_list.h"
#include "platform_timer.h"
#include "platform_memory.h"
#include "platform_mutex.h"
#include "platform_thread.h"
#include "http_defconfig.h"
#include "network.h"
#include "random.h"
#include "http_error.h"
#include "http_log.h"


typedef struct http_client {
    http_list_t                         list;
    http_interceptor_t                  *interceptor;
    http_connect_params_t               *connect_params;
    http_request_method_t               method;
    uint32_t                            interest_event;
    http_event_t                        *event;
    size_t                              process;
    size_t                              total;
    void                                *data;
    HTTP_GENERAL_FLAG;
} http_client_t;

int http_client_init(const char *ca);
void http_client_wait_exit(void);
void http_client_exit(void);
http_client_t *http_client_lease(void);
void http_client_release(http_client_t *c);
int http_client_get(const char *url, http_event_cb_t cb);
int http_client_post(const char *url, void *data ,http_event_cb_t cb);
void http_client_set_interest_event(http_client_t *c, http_event_type_t event);

void http_client_set_method(http_client_t *c, http_request_method_t method);
void http_client_set_data(http_client_t *c, void *data);

http_client_t *http_client_get_persistent(http_client_t *c, const char *url, http_event_cb_t cb);

#endif /* _HTTPCLIENT_H_ */
