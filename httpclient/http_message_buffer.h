/*
 * @Author: jiejie
 * @Github: https://github.com/jiejieTop
 * @Date: 2020-05-05 17:22:22
 * @LastEditTime: 2020-05-12 21:49:56
 * @Description: the code belongs to jiejie, please keep the author information and source code according to the license.
 */

#ifndef _HTTP_MESSAGE_BUFFER_H_
#define _HTTP_MESSAGE_BUFFER_H_

#include <stddef.h>
#include <http_utils.h>

typedef struct http_message_buffer {
    size_t              length;     /* number of bytes allocated */
    size_t              used;       /* strlen(data) + 1 */
    char                *data;      /* contents: NUL-terminated string */
} http_message_buffer_t;


http_message_buffer_t *http_message_buffer_init(size_t size);
int http_message_buffer_reinit(http_message_buffer_t *buf);
void http_message_buffer_grow(http_message_buffer_t *buf, size_t newsize);
void http_message_buffer_release(http_message_buffer_t *buf);
void http_message_buffer_concat(http_message_buffer_t *buf, ...);
void http_message_buffer_cover(http_message_buffer_t *buf, ...);
void http_message_buffer_append(http_message_buffer_t *buf, const char *str, size_t len);
int http_message_buffer_pointer(http_message_buffer_t *buf, const char *str, size_t len);

char *http_message_buffer_get_data(http_message_buffer_t *buf);
size_t http_message_buffer_get_used(http_message_buffer_t *buf);
size_t http_message_buffer_get_length(http_message_buffer_t *buf);

#endif // !_HTTP_MESSAGE_BUFFER_H_
