/*
 * @Author: jiejie
 * @Github: https://github.com/jiejieTop
 * @Date: 2019-12-27 03:25:58
 * @LastEditTime: 2020-04-27 23:38:34
 * @Description: the code belongs to jiejie, please keep the author information and source code according to the license.
 */
#ifndef _HTTP_LOG_H_
#define _HTTP_LOG_H_

#include "http_defconfig.h"

#ifdef LOG_IS_SALOF
    #include "salof.h"

    #define HTTP_LOG_D(fmt, ...)   LOG_DEBUG(fmt, ##__VA_ARGS__)
    #define HTTP_LOG_I(fmt, ...)   LOG_INFO(fmt, ##__VA_ARGS__)
    #define HTTP_LOG_W(fmt, ...)   LOG_WARN(fmt, ##__VA_ARGS__)
    #define HTTP_LOG_E(fmt, ...)   LOG_ERR(fmt, ##__VA_ARGS__)
    #define http_log_init   salof_init
#else
    #include <stdio.h>
    
    #define BASE_LEVEL      (0)
    #define ERR_LEVEL       (BASE_LEVEL + 1)
    #define WARN_LEVEL      (ERR_LEVEL + 1)
    #define INFO_LEVEL      (WARN_LEVEL + 1)
    #define DEBUG_LEVEL     (INFO_LEVEL + 1)

#if LOG_LEVEL < DEBUG_LEVEL
    #define HTTP_LOG_D(fmt, ...)
#else
    #define HTTP_LOG_D(fmt, ...)     { printf(fmt, ##__VA_ARGS__); printf("\n");}
#endif

#if LOG_LEVEL < INFO_LEVEL
    #define HTTP_LOG_I(fmt, ...)
#else
    #define HTTP_LOG_I(fmt, ...)      { printf(fmt, ##__VA_ARGS__); printf("\n");}
#endif

#if LOG_LEVEL < WARN_LEVEL
    #define HTTP_LOG_W(fmt, ...)
#else
    #define HTTP_LOG_W(fmt, ...)      { printf(fmt, ##__VA_ARGS__); printf("\n");}
#endif

#if LOG_LEVEL < ERR_LEVEL
    #define HTTP_LOG_E(fmt, ...)
#else
    #define HTTP_LOG_E(fmt, ...)       { printf(fmt, ##__VA_ARGS__); printf("\n");}
#endif

#if LOG_LEVEL < BASE_LEVEL
    #define HTTP_LOG(fmt, ...)
#else
    #define HTTP_LOG(fmt, ...)           { printf(fmt, ##__VA_ARGS__); printf("\n");}
#endif

    #define http_log_init()
#endif

#endif /* _LOG_H_ */
