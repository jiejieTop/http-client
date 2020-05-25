/*
 * @Author: jiejie
 * @Github: https://github.com/jiejieTop
 * @Date: 2020-05-05 19:36:36
 * @LastEditTime: 2020-05-25 21:08:29
 * @Description: the code belongs to jiejie, please keep the author information and source code according to the license.
 */

#include <http_response.h>
#include <http_log.h>
#include <http_error.h>


int http_response_init(http_response_t *rsp)
{
    HTTP_ROBUSTNESS_CHECK(rsp , HTTP_NULL_VALUE_ERROR);

    rsp->message = http_message_buffer_init(HTTP_MESSAGE_BUFFER_GROWTH);

    RETURN_ERROR(HTTP_SUCCESS_ERROR);
}

int http_response_release(http_response_t *rsp)
{
    HTTP_ROBUSTNESS_CHECK(rsp , HTTP_NULL_VALUE_ERROR);

    http_message_buffer_release(rsp->message);

    rsp->message = NULL;

    RETURN_ERROR(HTTP_SUCCESS_ERROR);
}


size_t http_response_get_length(http_response_t *rsp)
{
    HTTP_ROBUSTNESS_CHECK(rsp , 0);
    return rsp->length;
}

void http_response_set_length(http_response_t *rsp, size_t length)
{
    HTTP_ROBUSTNESS_CHECK(rsp, HTTP_VOID);
    rsp->length = length;
}

uint32_t http_response_get_offset(http_response_t *rsp)
{
    HTTP_ROBUSTNESS_CHECK(rsp , 0);
    return rsp->offset;
}

void http_response_set_offset(http_response_t *rsp, uint32_t offset)
{
    HTTP_ROBUSTNESS_CHECK(rsp, HTTP_VOID);
    rsp->offset = offset;
}


http_response_status_t http_response_get_status(http_response_t *rsp)
{
    HTTP_ROBUSTNESS_CHECK(rsp , HTTP_NULL_VALUE_ERROR);
    return rsp->status;
}

void http_response_set_status(http_response_t *rsp, unsigned int status)
{
    HTTP_ROBUSTNESS_CHECK((rsp && status), HTTP_VOID);
    rsp->status = (http_response_status_t)status;
}

/* get all the data of the response message, length, user, data */
http_message_buffer_t *http_response_get_message(http_response_t *rsp)
{
    HTTP_ROBUSTNESS_CHECK(rsp, NULL);
    return rsp->message;
}
/* get all the data of the response message data, including the start line and header of the response message */
char *http_response_get_message_data(http_response_t *rsp)
{
    HTTP_ROBUSTNESS_CHECK(rsp, NULL);
    return rsp->message->data;
}

/* get the body data of the response message, skip the header field */
char *http_response_get_message_body(http_response_t *rsp)
{
    HTTP_ROBUSTNESS_CHECK(rsp, NULL);
    return (rsp->message->data + rsp->offset);
}

/* get all the data of the response message, including the start line and header of the response message */
size_t http_response_get_message_len(http_response_t *rsp)
{
    HTTP_ROBUSTNESS_CHECK(rsp, 0);
    return rsp->message->used;
}








