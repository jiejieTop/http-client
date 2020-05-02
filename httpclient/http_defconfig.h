/*
 * @Author: jiejie
 * @Github: https://github.com/jiejieTop
 * @Date: 2020-02-25 03:36:09
 * @LastEditTime: 2020-05-02 11:28:36
 * @Description: the code belongs to jiejie, please keep the author information and source code according to the license.
 */

#ifndef _DEFCONFIG_H_
#define _DEFCONFIG_H_

#include "http_config.h"

#ifndef HTTP_DEFAULT_BUF_SIZE
    #define     HTTP_DEFAULT_BUF_SIZE               1024
#endif // !HTTP_DEFAULT_BUF_SIZE

#ifndef HTTP_DEFAULT_CMD_TIMEOUT
    #define     HTTP_DEFAULT_CMD_TIMEOUT            4000
#endif // !HTTP_DEFAULT_CMD_TIMEOUT

#ifndef HTTP_MAX_CMD_TIMEOUT
    #define     HTTP_MAX_CMD_TIMEOUT                20000
#endif // !HTTP_MAX_CMD_TIMEOUT

#ifndef HTTP_MIN_CMD_TIMEOUT
    #define     HTTP_MIN_CMD_TIMEOUT                1000
#endif // !HTTP_MIN_CMD_TIMEOUT

#ifndef HTTP_KEEP_ALIVE_INTERVAL
    #define     HTTP_KEEP_ALIVE_INTERVAL            100      // unit: second
#endif // !HTTP_KEEP_ALIVE_INTERVAL

#ifndef HTTP_RECONNECT_DEFAULT_DURATION
    #define     HTTP_RECONNECT_DEFAULT_DURATION     1000
#endif // !HTTP_RECONNECT_DEFAULT_DURATION

#ifndef HTTP_THREAD_STACK_SIZE
    #define     HTTP_THREAD_STACK_SIZE              4096
#endif // !HTTP_THREAD_STACK_SIZE

#ifndef HTTP_THREAD_PRIO
    #define     HTTP_THREAD_PRIO                    5
#endif // !HTTP_THREAD_PRIO

#ifndef HTTP_THREAD_TICK
    #define     HTTP_THREAD_TICK                    50
#endif // !HTTP_THREAD_TICK


#ifdef HTTP_NETWORK_TYPE_TLS

#ifndef HTTP_TLS_HANDSHAKE_TIMEOUT
    #define HTTP_TLS_HANDSHAKE_TIMEOUT  (5 * 1000)
#endif // !HTTP_TLS_HANDSHAKE_TIMEOUT
    
    #include "mbedtls/config.h"
    #include "mbedtls/ssl.h"
    #include "mbedtls/entropy.h"
    #include "mbedtls/net_sockets.h"
    #include "mbedtls/ctr_drbg.h"
    #include "mbedtls/error.h"
    #include "mbedtls/debug.h"
#endif /* HTTP_NETWORK_TYPE_TLS */

#endif /* _DEFCONFIG_H_ */

