/*
 * @Author: jiejie
 * @Github: https://github.com/jiejieTop
 * @Date: 2020-05-21 20:59:36
 * @LastEditTime: 2020-05-26 21:06:40
 * @Description: the code belongs to jiejie, please keep the author information and source code according to the license.
 */ 

#ifndef _HTTP_WQ_H_
#define _HTTP_WQ_H_

#include <http_list.h>
#include <platform_thread.h>
#include <platform_mutex.h>
#include <http_defconfig.h>

#ifdef HTTP_USING_WORK_QUEUE

typedef void (*http_worker_func_t)(void *);

typedef struct http_wq {
    int                 loop;
    platform_thread_t   *thread;
    platform_mutex_t    mutex;
    http_list_t         list;
} http_wq_t;

typedef struct http_wq_pool {
    int                 cpus;
    int                 ring;
    http_wq_t           *wq;         //multi workq
} http_wq_pool_t;

typedef struct worker {
    http_list_t         entry;
    http_worker_func_t  func;
    void                *data;
    http_wq_t           *wq;           //root
} http_worker_t;

int http_wq_pool_init(void);
void http_wq_pool_deinit(void);
void http_wq_wait_exit(void);
int http_wq_add_task(http_worker_func_t func, void *data, size_t len);

#endif // HTTP_USING_WORK_QUEUE

#endif // !_HTTP_WQ_H_

