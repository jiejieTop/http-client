/*
 * @Author: jiejie
 * @Github: https://github.com/jiejieTop
 * @Date: 2020-05-12 13:52:36
 * @LastEditTime: 2020-05-13 19:04:36
 * @Description: the code belongs to jiejie, please keep the author information and source code according to the license.
 */

#ifndef _HTTP_EVENT_H_
#define _HTTP_EVENT_H_

#include <stdint.h>
#include <stddef.h>

typedef int (*http_event_cb_t)(void *e);

typedef enum http_event_type {
    http_event_type_on_error = 0x00,
    http_event_type_on_connect,
    http_event_type_on_request,
    http_event_type_on_response,
    http_event_type_on_headers,
    http_event_type_on_body,
    http_event_type_on_submit,
    http_event_type_on_release,
    http_event_type_all
} http_event_type_t;

typedef struct http_event {
    http_event_type_t       type;           /* http_event_type_t type */
    void                    *context;
    void                    *data;
    size_t                  len;
    http_event_cb_t         cb;
} http_event_t;

http_event_t *http_event_init(void);
void http_event_release(http_event_t *event);
int http_event_register(http_event_t *event, http_event_cb_t cb);
int http_event_dispatch(http_event_t *event, http_event_type_t type, void *context, void *data, size_t len);


#endif // !_HTTP_EVENT_H_
