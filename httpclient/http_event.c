/*
 * @Author: jiejie
 * @Github: https://github.com/jiejieTop
 * @Date: 2020-05-12 13:52:28
 * @LastEditTime: 2020-05-13 15:27:03
 * @Description: the code belongs to jiejie, please keep the author information and source code according to the license.
 */

#include <http_event.h>
#include <http_log.h>
#include <http_error.h>
#include <http_utils.h>
#include <platform_memory.h>

http_event_t *http_event_init(void)
{
    int len = 0;
    http_event_t *event;

    len = sizeof(http_event_t);

    event = (http_event_t *)platform_memory_alloc(len);
    memset(event, 0, len);

    return event;
}

void http_event_release(http_event_t *event)
{
    HTTP_ROBUSTNESS_CHECK(event, HTTP_VOID);
    platform_memory_free(event);
    event = NULL;
}


int http_event_register(http_event_t *event, http_event_cb_t cb)
{
    HTTP_ROBUSTNESS_CHECK((event && cb), HTTP_NULL_VALUE_ERROR);
    event->cb = cb;
    RETURN_ERROR(HTTP_SUCCESS_ERROR);
}

int http_event_dispatch(http_event_t *event, http_event_type_t type, void *context, void *data, size_t len)
{
    HTTP_ROBUSTNESS_CHECK(event, HTTP_NULL_VALUE_ERROR);

    if (event->cb) {
        event->type = type;
        event->context = context;
        event->data = data;
        event->len = len;
        return event->cb(event);
    }
    RETURN_ERROR(HTTP_SUCCESS_ERROR);
}
