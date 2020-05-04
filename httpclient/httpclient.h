/*
 * @Author: jiejie
 * @Github: https://github.com/jiejieTop
 * @Date: 2019-12-09 21:31:25
 * @LastEditTime: 2020-05-04 11:38:56
 * @Description: the code belongs to jiejie, please keep the author information and source code according to the license.
 */
#ifndef _HTTPCLIENT_H_
#define _HTTPCLIENT_H_

#include <stdio.h>
#include <string.h>

#include "http_parser.h"
#include "http_utils.h"
#include "http_connector.h"
#include "http_url_parser.h"
#include "http_interceptor.h"

#include "http_list.h"
#include "platform_timer.h"
#include "platform_memory.h"
#include "platform_mutex.h"
#include "platform_thread.h"
#include "http_defconfig.h"
#include "network.h"
#include "random.h"
#include "http_error.h"
#include "http_log.h"


typedef struct http_client {
    http_connect_status_t               status;
    http_list_t                         connect_pool_list;
    // network_t                   *network;
    platform_mutex_t                    write_lock;
    platform_mutex_t                    global_lock;

} http_client_t;




#endif /* _HTTPCLIENT_H_ */
