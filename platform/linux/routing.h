/*
 * @Author: jiejie
 * @Github: https://github.com/jiejieTop
 * @Date: 2020-05-15 16:24:49
 * @LastEditTime: 2020-05-16 10:52:48
 * @Description: the code belongs to jiejie, please keep the author information and source code according to the license.
 */ 

#ifndef _ROUTING_H_
#define _ROUTING_H_

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

typedef struct routing {
    uint32_t            hash;
    char                *host;
    char                *ip;
} routing_t;

void routing_record(const char* host, const char *ip);
char *routing_search(const char* host);
 


#endif // !_ROUTING_H_

