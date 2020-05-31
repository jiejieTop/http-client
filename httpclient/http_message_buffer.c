/*
 * @Author: jiejie
 * @Github: https://github.com/jiejieTop
 * @Date: 2020-05-05 17:21:58
 * @LastEditTime: 2020-05-20 20:20:08
 * @Description: the code belongs to jiejie, please keep the author information and source code according to the license.
 */

#include <http_message_buffer.h>
#include <http_log.h>
#include <http_error.h>
#include <platform_memory.h>

/* grows for given size */
void http_message_buffer_grow(http_message_buffer_t *buf, size_t newsize)
{
    if (newsize > buf->length) {
        /* if it's not big enough already... */
        buf->length = ((newsize / HTTP_MESSAGE_BUFFER_GROWTH) + 1) * HTTP_MESSAGE_BUFFER_GROWTH;

        buf->data = platform_memory_realloc(buf->data, buf->length);
    }
}

http_message_buffer_t *http_message_buffer_init(size_t size)
{
    http_message_buffer_t *buf;

    if (0 == size)
        size = HTTP_MESSAGE_BUFFER_GROWTH;

    buf = (http_message_buffer_t *)platform_memory_alloc(sizeof(http_message_buffer_t));
    
    buf->data = platform_memory_alloc(size);
    buf->data[0] = '\0';
    buf->length = size;
    buf->used = 1;      /* use 1 byte space in advance to prevent overflow */     
    
    return buf;
}

int http_message_buffer_reinit(http_message_buffer_t *buf)
{
    HTTP_ROBUSTNESS_CHECK(buf, HTTP_NULL_VALUE_ERROR);

    if (0 == buf->length) {
        buf->length = HTTP_MESSAGE_BUFFER_GROWTH;
        buf->data = platform_memory_alloc(HTTP_MESSAGE_BUFFER_GROWTH);
        HTTP_ROBUSTNESS_CHECK((buf->data), HTTP_MEM_NOT_ENOUGH_ERROR);
    }
    
    memset(buf->data, 0, buf->length);
    buf->data[0] = '\0';
    buf->used = 1;

    RETURN_ERROR(HTTP_SUCCESS_ERROR);
}

void http_message_buffer_release(http_message_buffer_t *buf)
{
    if (NULL == buf)
        return;

    if (0 != buf->length) {
        buf->length = 0;
        platform_memory_free((void*)buf->data);
    }
        
    buf->data = NULL;
    buf->used = 0;
    
    platform_memory_free((void*)buf);
    buf = NULL;
}

/* concatenate all given strings on to the end of the buf. the strings must all be NULL terminated */
void http_message_buffer_concat(http_message_buffer_t *buf, ...)
{
    va_list ap;
    size_t total;

    va_start(ap, buf);
    total = buf->used + http_utils_count_concat(&ap);
    va_end(ap);    

    /* grow the buffer */
    http_message_buffer_grow(buf, total);
    
    va_start(ap, buf);    
    http_utils_concat(buf->data + buf->used - 1, &ap);
    va_end(ap); 
    
    buf->used = total;
    buf->data[total - 1] = '\0';
}

void http_message_buffer_append(http_message_buffer_t *buf, const char *str, size_t len) 
{
    http_message_buffer_grow(buf, buf->used + len);
    memcpy(buf->data + buf->used - 1, str, len);
    buf->used += len;
    buf->data[buf->used - 1] = '\0';
}


void http_message_buffer_cover(http_message_buffer_t *buf, ...)
{
    va_list ap;
    size_t total;

    va_start(ap, buf);
    total = 1 + http_utils_count_concat(&ap);
    va_end(ap);    

    /* grow the buffer */
    http_message_buffer_grow(buf, total);
    
    va_start(ap, buf);    
    http_utils_concat(buf->data, &ap);
    va_end(ap); 
    
    buf->used = total;
    buf->data[total - 1] = '\0';
}

int http_message_buffer_pointer(http_message_buffer_t *buf, const char *str, size_t len)
{
    HTTP_ROBUSTNESS_CHECK((buf && str && len), HTTP_NULL_VALUE_ERROR);

    platform_memory_free(buf->data);
    buf->length = 0;

    buf->data = (char*)str;
    buf->used += len;

    RETURN_ERROR(HTTP_SUCCESS_ERROR);
}


char *http_message_buffer_get_data(http_message_buffer_t *buf)
{
    HTTP_ROBUSTNESS_CHECK(buf, NULL);
    return buf->data;
}

size_t http_message_buffer_get_used(http_message_buffer_t *buf)
{
    HTTP_ROBUSTNESS_CHECK(buf, 0);
    return buf->used;
}

size_t http_message_buffer_get_length(http_message_buffer_t *buf)
{
    HTTP_ROBUSTNESS_CHECK(buf, 0);
    return buf->length;
}