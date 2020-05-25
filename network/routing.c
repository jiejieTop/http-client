/*
 * @Author: jiejie
 * @Github: https://github.com/jiejieTop
 * @Date: 2020-05-15 16:24:37
 * @LastEditTime: 2020-05-25 21:04:55
 * @Description: the code belongs to jiejie, please keep the author information and source code according to the license.
 */ 
#include <string.h>
#include <routing.h>
#include <platform_memory.h>
#include <platform_mutex.h>

#ifndef ROUTING_TABLE_SIZE
    #define ROUTING_TABLE_SIZE   10
#endif // !ROUTING_TABLE_SIZE

// #define ROUTING_STRICT_HANDLING
#ifdef ROUTING_STRICT_HANDLING
    static platform_mutex_t *_routing_mutex = NULL;
#endif // !ROUTING_STRICT_HANDLING

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

static int _routing_match(const char *str1, const char *str2, int n)
{
    int i = 0;
    int c1, c2;
    do {
        c1 = *str1++;
        c2 = *str2++;
        i++;
    } while(((c1 == c2) && c1) && (i < n));

    return c1 - c2;
}

void routing_record(const char *host, const char *ip)
{
    routing_t *routing;

#ifdef ROUTING_STRICT_HANDLING
    if (NULL == _routing_mutex) {
        _routing_mutex = platform_memory_alloc(sizeof(platform_mutex_t));
        if (NULL != _routing_mutex) {
            platform_mutex_init(_routing_mutex);
        }
    }
#endif

    int host_len = strlen(host);
    int ip_len = strlen(ip);

    if (_routing_index >= ROUTING_TABLE_SIZE - 1)
        _routing_index = 0;

    routing = &_routing_table[_routing_index];

#ifdef ROUTING_STRICT_HANDLING
    platform_mutex_lock(_routing_mutex);
#endif

    _routing_index ++;          /* I don't need mutex protection, fast ++ */

#ifdef ROUTING_STRICT_HANDLING
    platform_mutex_unlock(_routing_mutex);
#endif

    if (routing->host != NULL) {
        platform_memory_free(routing->host);
        routing->host = NULL;
    }

    if (routing->ip != NULL) {
        platform_memory_free(routing->ip);
        routing->ip = NULL;
    }

    routing->host = platform_memory_alloc(host_len);
    routing->ip = platform_memory_alloc(ip_len);

    if ((NULL != routing->host) && (NULL != routing->ip)) {
        
        routing->hash = _routing_hash(host);

        memcpy(routing->host, host, host_len);
        memcpy(routing->ip, ip, ip_len);

        routing->host[host_len] = '\0';
        routing->ip[ip_len] = '\0';

    } else {

#ifdef ROUTING_STRICT_HANDLING
    platform_mutex_lock(_routing_mutex);
#endif
        _routing_index --;      /* ailed, recover */
        
#ifdef ROUTING_STRICT_HANDLING
    platform_mutex_unlock(_routing_mutex);
#endif
    }

}

char *routing_search(const char* host)
{
    uint8_t index = 0;
    routing_t *routing;
    int len = sizeof(host);
    uint32_t hash = _routing_hash(host);

    for (index = 0; index < ROUTING_TABLE_SIZE; index ++) {

        routing = &_routing_table[index];
        
        if (hash == routing->hash) {
            if (_routing_match(host, routing->host, len) == 0) {
                return routing->ip;
            }
        }
    }
    return NULL;
}



