/*
 * @Author: jiejie
 * @Github: https://github.com/jiejieTop
 * @Date: 2020-05-15 16:24:37
 * @LastEditTime: 2020-05-16 15:56:57
 * @Description: the code belongs to jiejie, please keep the author information and source code according to the license.
 */ 
#include <string.h>
#include <routing.h>
#include <platform_memory.h>

#ifndef ROUTING_TABLE_SIZE
    #define ROUTING_TABLE_SIZE   10
#endif // !ROUTING_TABLE_SIZE

static uint8_t  _routing_index = 0;
static routing_t _routing_table[ROUTING_TABLE_SIZE] = {0};

static int _routing_to_lower(int c)
{
    if ((c >= 'A') && (c <= 'Z'))
        return c + ('a' - 'A');
    return c;
}

static uint32_t _routing_hash(const char* str)
{
    int tmp, c = *str;
    uint32_t hash = 0, seed = 0xb433e5c6;  /* 'jiejie' string hash */  
    
    while(*str) {
        tmp = _routing_to_lower(c);
        hash = (hash ^ seed) + tmp;
        str++;
        c = *str;
    }
    return hash;
}

static int _routing_match(const char *str, const char *cmd)
{
    int c1, c2;

    do {
        c1 = _routing_to_lower(*str++);
        c2 = _routing_to_lower(*cmd++);
    } while((c1 == c2) && c1);

    return c1 - c2;
}

void routing_record(const char *host, const char *ip)
{
    routing_t *routing;

    int host_len = strlen(host) + 1;
    int ip_len = strlen(ip) + 1;

    if (_routing_index >= ROUTING_TABLE_SIZE - 1)
        _routing_index = 0;

    routing = &_routing_table[_routing_index];

    routing->host = platform_memory_alloc(host_len);
    routing->ip = platform_memory_alloc(host_len);

    if ((NULL != routing->host) && (NULL != routing->ip)) {
        
        routing->hash = _routing_hash(host);

        memcpy(routing->host, host, host_len);
        memcpy(routing->ip, ip, ip_len);

        routing->host[host_len] = '\0';
        routing->ip[ip_len] = '\0';

        _routing_index ++;
    }
}

char *routing_search(const char* host)
{
    uint8_t index = 0;
    routing_t *routing;
    uint32_t hash = _routing_hash(host);

    for (index = 0; index < ROUTING_TABLE_SIZE; index ++) {

        routing = &_routing_table[index];

        if (hash == routing->hash) {
            if (_routing_match(host, routing->host) == 0) {
                return routing->ip;
            }
        }
    }
    return NULL;
}



