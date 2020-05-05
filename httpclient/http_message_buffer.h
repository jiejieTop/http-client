/*
 * @Author: jiejie
 * @Github: https://github.com/jiejieTop
 * @Date: 2020-05-05 17:22:22
 * @LastEditTime: 2020-05-05 19:49:37
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
void http_message_buffer_concat(http_message_buffer_t *buf, ...);

#endif // !_HTTP_MESSAGE_BUFFER_H_
