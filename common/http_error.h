/*
 * @Author: jiejie
 * @Github: https://github.com/jiejieTop
 * @Date: 2019-12-15 00:42:16
 * @LastEditTime: 2020-05-26 21:21:33
 * @Description: the code belongs to jiejie, please keep the author information and source code according to the license.
 */
#ifndef _HTTP_ERROR_H_
#define _HTTP_ERROR_H_

typedef enum http_error {
    HTTP_SOCKET_FAILED_ERROR                                = -0x0007,      /* socket fd failed */
    HTTP_SOCKET_UNKNOWN_HOST_ERROR                          = -0x0006,      /* socket unknown host ip or domain */ 
    HTTP_SEND_MESSAGE_ERROR                                 = -0x0005,      /* http send a message failed */
    HTTP_CONNECT_FAILED_ERROR                               = -0x0004,      /* http connect failed */
    HTTP_MEM_NOT_ENOUGH_ERROR                               = -0x0003,      /* http memory not enough */
    HTTP_NULL_VALUE_ERROR                                   = -0x0002,      /* http value is null */
    HTTP_FAILED_ERROR                                       = -0x0001,      /* failed */
    HTTP_SUCCESS_ERROR                                      = 0x0000        /* success */
} http_error_t;

#define RETURN_ERROR(x) { return x; }

#endif
