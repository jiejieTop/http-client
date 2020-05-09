/*
 * @Author: jiejie
 * @Github: https://github.com/jiejieTop
 * @Date: 2020-05-05 19:36:42
 * @LastEditTime: 2020-05-09 19:37:16
 * @Description: the code belongs to jiejie, please keep the author information and source code according to the license.
 */

#ifndef _HTTP_RESPONSE_H_
#define _HTTP_RESPONSE_H_

#include <http_general.h>
#include <http_message_buffer.h>

typedef struct http_response {
    uint16_t                        status  : 15;   /* response status code */
    uint16_t                        chunked : 1;    /* response status code */
    uint16_t                        length;         /* content length */
    uint16_t                        offset;         /* data offset, skip header */
    http_message_buffer_t           *message;
} http_response_t;

int http_response_init(http_response_t *rsp);

#endif // !_HTTP_RESPONSE_H_
