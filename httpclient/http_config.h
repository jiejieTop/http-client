/*
 * @Author: jiejie
 * @Github: https://github.com/jiejieTop
 * @Date: 2019-12-15 00:42:16
 * @LastEditTime : 2020-01-16 00:39:23
 * @Description: the code belongs to jiejie, please keep the author information and source code according to the license.
 */
#ifndef _HTTP_CONFIG_H_
#define _HTTP_CONFIG_H_

#define     HTTP_NO                             0
#define     HTTP_YES                            1

#define     HTTP_DEFAULT_BUF_SIZE               1024
#define     HTTP_DEFAULT_CMD_TIMEOUT            4000
#define     HTTP_MAX_CMD_TIMEOUT                20000
#define     HTTP_MIN_CMD_TIMEOUT                1000
#define     HTTP_KEEP_ALIVE_INTERVAL            20      // unit: second
#define     HTTP_RECONNECT_DEFAULT_DURATION     1000
#define     HTTP_THREAD_STACK_SIZE              2048
#define     HTTP_THREAD_PRIO                    10
#define     HTTP_THREAD_TICK                    50


#define     HTTP_NETWORK_TYPE_TLS               HTTP_YES
#define     HTTP_TLS_HANDSHAKE_TIMEOUT          (5 * 1000)

#if HTTP_NETWORK_TYPE_TLS
    #include "mbedtls/config.h"
    #include "mbedtls/ssl.h"
    #include "mbedtls/entropy.h"
    #include "mbedtls/net_sockets.h"
    #include "mbedtls/ctr_drbg.h"
    #include "mbedtls/error.h"
    #include "mbedtls/debug.h"
#endif /* HTTP_NETWORK_TYPE_TLS */

#endif /* _HTTP_CONFIG_H_ */
