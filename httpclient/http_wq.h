/*
 * @Author: jiejie
 * @Github: https://github.com/jiejieTop
 * @Date: 2020-05-21 20:59:36
 * @LastEditTime: 2020-05-22 23:02:49
 * @Description: the code belongs to jiejie, please keep the author information and source code according to the license.
 */ 

#ifndef _HTTP_WQ_H_
#define _HTTP_WQ_H_

#include <http_list.h>
#include <platform_thread.h>
#include <platform_mutex.h>

typedef struct http_wq {
    int loop;
    platform_thread_t           *thread;
    platform_mutex_t            mutex;
    http_list_t                 list;
} http_wq_t;

typedef struct http_wq_pool {
    int             cpus;
    int             ring;
    http_wq_t       *wq;         //multi workq
} http_wq_pool_t;

int http_wq_pool_init(void);


#endif // !_HTTP_WQ_H_

