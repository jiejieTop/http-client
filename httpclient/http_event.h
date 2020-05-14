/*
 * @Author: jiejie
 * @Github: https://github.com/jiejieTop
 * @Date: 2020-05-12 13:52:36
 * @LastEditTime: 2020-05-14 22:49:46
 * @Description: the code belongs to jiejie, please keep the author information and source code according to the license.
 */

#ifndef _HTTP_EVENT_H_
#define _HTTP_EVENT_H_

#include <stdint.h>
#include <stddef.h>

typedef int (*http_event_cb_t)(void *e);

typedef enum http_event_type {
    http_event_type_on_error = ~0x0,
    http_event_type_on_connect = 0x0001 << 0,
    http_event_type_on_request = 0x0001 << 1,
    http_event_type_on_response = 0x0001 << 2,
    http_event_type_on_headers = 0x0001 << 3,
    http_event_type_on_body = 0x0001 << 4,
    http_event_type_on_submit = 0x0001 << 5,
    http_event_type_on_release = 0x0001 << 6,
    http_event_type_all = 0x0001 << 7
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
