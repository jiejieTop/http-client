/*
 * @Author: jiejie
 * @Github: https://github.com/jiejieTop
 * @Date: 2020-05-15 16:24:49
 * @LastEditTime: 2020-05-15 17:46:51
 * @Description: the code belongs to jiejie, please keep the author information and source code according to the license.
 */ 

#ifndef _HTTP_ROUTING_H_
#define _HTTP_ROUTING_H_

#include <stdint.h>
#include <stddef.h>

/**
 * --------------------------------------------------
 * |    hash    |       host        |       ip      |
 * --------------------------------------------------
 * |   0x1234   |  www.jiedev.com   | 192.168.0.122 |
 * |   0x5678   |  www.github.com   | 192.168.0.123 |
 * |   xxxxxx   |  xxxxxxxxxxxxxx   | xxx.xxx.x.xxx |
 * --------------------------------------------------
 */

typedef struct http_routing {
    uint32_t            hash;
    char                *host;
    char                *ip;
} http_routing_t;



 


#endif // !_HTTP_ROUTING_H_

