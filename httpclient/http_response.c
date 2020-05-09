/*
 * @Author: jiejie
 * @Github: https://github.com/jiejieTop
 * @Date: 2020-05-05 19:36:36
 * @LastEditTime: 2020-05-09 19:39:25
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



