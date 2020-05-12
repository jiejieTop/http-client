/*
 * @Author: jiejie
 * @Github: https://github.com/jiejieTop
 * @Date: 2020-05-12 13:52:36
 * @LastEditTime: 2020-05-12 22:03:55
 * @Description: the code belongs to jiejie, please keep the author information and source code according to the license.
 */

#ifndef _HTTP_EVENT_H_
#define _HTTP_EVENT_H_

#include <stdint.h>
#include <stddef.h>

typedef enum http_event_type {
    http_event_type_on_error = 0x00,
    http_event_type_on_connect,
    http_event_type_on_request,
    http_event_type_on_response,
    http_event_type_on_headers,
    http_event_type_on_body,
    http_event_type_on_release,
    http_event_type_all
} http_event_type_t;

typedef struct http_event {
    http_event_type_t       type;           /* http_event_type_t type */
    void                    *context;
    void                    *data;
    size_t                  len;
} http_event_t;

typedef int (*http_event_cb_t)(http_event_t *e);

#define HTTP_EVENT_CB_DEFINE    \
    http_event_cb_t     http_event[http_event_type_all]



#endif // !_HTTP_EVENT_H_
