/*
 * @Author: jiejie
 * @Github: https://github.com/jiejieTop
 * @Date: 2020-05-21 20:59:27
 * @LastEditTime: 2020-05-22 23:04:32
 * @Description: the code belongs to jiejie, please keep the author information and source code according to the license.
 */ 
#include <http_wq.h>
#include <http_error.h>
#include <http_utils.h>
#include <platform_memory.h>

static http_wq_pool_t _wq_pool;

static void _wq_thread(void *arg)
{
    http_wq_t *wq = (http_wq_t *)arg;

    while (wq->loop) {

    }
}

static int _wq_init(http_wq_t *wq)
{
    if (!wq) {
        return -1;
    }

    http_list_init(&wq->list);
    wq->loop = 1;

    platform_mutex_init(&wq->mutex);

    wq->thread= platform_thread_init("_wq_thread", _wq_thread, wq, HTTP_THREAD_STACK_SIZE, HTTP_THREAD_PRIO, HTTP_THREAD_TICK);

    return 0;
}

int http_wq_pool_init(void)
{
    int i;

#ifdef __linux__
    #include <sys/sysinfo.h>
    int cpus = get_nprocs_conf();
#else
    int cpus = 1;
#endif // __linux__

    if (cpus <= 0) {
        HTTP_LOG_E("get nprocs conf failed!\n");
        return -1;
    }
    
   http_wq_t *wq = platform_memory_alloc(cpus * sizeof(http_wq_t));

    HTTP_ROBUSTNESS_CHECK(wq, HTTP_MEM_NOT_ENOUGH_ERROR)

    _wq_pool.wq = wq;
    for (i = 0; i < cpus; ++i, ++wq) {
        if (0 != _wq_init(wq)) {
            printf("_wq_init failed!\n");
            return -1;
        }
    }

    _wq_pool.cpus = cpus;
    _wq_pool.ring = 0;

    return 0;
}

