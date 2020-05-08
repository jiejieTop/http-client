/*
 * @Author: jiejie
 * @Github: https://github.com/jiejieTop
 * @Date: 2020-04-16 20:36:06
 * @LastEditTime: 2020-05-08 21:12:58
 * @Description: the code belongs to jiejie, please keep the author information and source code according to the license.
 */

#ifndef _HTTP_CONNECTOR_H_
#define _HTTP_CONNECTOR_H_

#include <http_list.h>
#include <http_log.h>
#include <http_utils.h>
#include <http_interceptor.h>

#ifndef HTTP_CONNECT_POOL_MAX_NUM
    #define HTTP_CONNECT_POOL_MAX_NUM   4
#endif

typedef enum http_connect_status {
    HTTP_CONNECT_STATUS_IDLE    =   0x00,   /* is idle */
    HTTP_CONNECT_STATUS_LEASE   =   0x01,   /* is lease */
    HTTP_CONNECT_STATUS_CONNECT =   0x02,   /* is connect */
    HTTP_CONNECT_STATUS_STABLE  =   0x04,   /* is stable */
    HTTP_CONNECT_STATUS_STALE   =   0x08,   /* is stale */
    HTTP_CONNECT_STATUS_DESTROY =   0x10    /* is destroy */
} http_connect_status_t;

#endif // !_HTTP_CONNECTOR_H_

