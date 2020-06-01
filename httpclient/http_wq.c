/*
 * @Author: jiejie
 * @Github: https://github.com/jiejieTop
 * @Date: 2020-05-21 20:59:27
 * @LastEditTime: 2020-05-28 17:23:47
 * @Description: the code belongs to jiejie, please keep the author information and source code according to the license.
 */ 
#include <http_wq.h>
#include <http_error.h>
#include <http_utils.h>
#include <platform_memory.h>

#ifdef HTTP_USING_WORK_QUEUE

static http_wq_pool_t _wq_pool;

static http_worker_t *_http_get_frist_worker(http_wq_t *wq)
{
    http_worker_t *w = NULL;

    platform_mutex_lock(&wq->mutex);
    w = HTTP_LIST_FIRST_ENTRY_OR_NULL(&wq->list, http_worker_t, entry);
    platform_mutex_unlock(&wq->mutex);

    return w;
}

static int _http_wq_worker_create(http_wq_t *wq, http_worker_func_t func, void *data, size_t len)
{
    http_worker_t *w = NULL;
    w = platform_memory_alloc(sizeof(http_worker_t));
    memset(w, 0, sizeof(http_worker_t));
    HTTP_ROBUSTNESS_CHECK(w, HTTP_MEM_NOT_ENOUGH_ERROR);

    w->wq = wq;
    w->func = func;
    w->data = data;

    platform_mutex_lock(&wq->mutex);
    http_list_add_tail(&w->entry, &wq->list);
    platform_mutex_unlock(&wq->mutex);
    
    platform_thread_start(wq->thread);
    
    RETURN_ERROR(HTTP_SUCCESS_ERROR);
}

static int _http_wq_worker_destroy(http_worker_t *w)
{
    http_wq_t *wq = NULL;
    HTTP_ROBUSTNESS_CHECK(w, HTTP_NULL_VALUE_ERROR);

    wq = w->wq;

    platform_mutex_lock(&wq->mutex);
    http_list_del(&w->entry);
    platform_mutex_unlock(&wq->mutex);

    platform_memory_free(w);

    RETURN_ERROR(HTTP_SUCCESS_ERROR);
}

static void _http_wq_thread(void *arg)
{
    http_worker_t *w = NULL;
    http_wq_t *wq = (http_wq_t *)arg;

    platform_thread_notice_enter(wq->thread);

    while (wq->loop) {
        w = _http_get_frist_worker(wq);

    start_working:
        if (NULL != w) {
            if (w->func) {
                w->func(w->data);
            }
            _http_wq_worker_destroy(w);

            if ((w = _http_get_frist_worker(wq)) != NULL) {
                goto start_working;
            }
        } else {
            platform_thread_stop(wq->thread);
        }
    }
    platform_thread_notice_exit(wq->thread);
}

static int _http_wq_init(http_wq_t *wq)
{
    HTTP_ROBUSTNESS_CHECK(wq, HTTP_NULL_VALUE_ERROR);

    wq->loop = 1;
    http_list_init(&wq->list);
    
    platform_mutex_init(&wq->mutex);

    wq->thread= platform_thread_init("_http_wq_thread", _http_wq_thread, wq, HTTP_THREAD_STACK_SIZE, HTTP_THREAD_PRIO, HTTP_THREAD_TICK);

    if (NULL != wq->thread) {
        platform_thread_startup(wq->thread);
        platform_thread_start(wq->thread);       /* start run mqtt thread */
    }
    
    return 0;
}

static void _http_wq_deinit(http_wq_t *wq)
{
    http_worker_t *w;
    HTTP_ROBUSTNESS_CHECK(wq, HTTP_VOID);

    while ((w = _http_get_frist_worker(wq))) {
        _http_wq_worker_destroy(w);
    }

    wq->loop = 0;

    platform_thread_destroy(wq->thread);
    wq->thread = NULL;
}

static void _http_wq_wait(http_wq_t *wq)
{
    http_worker_t *w;
    HTTP_ROBUSTNESS_CHECK(wq, HTTP_VOID);

    if (NULL != wq->thread) {
        platform_thread_startup(wq->thread);
        platform_thread_start(wq->thread);       /* start run mqtt thread */
    }

    wq->loop = 0;
    platform_thread_wait_exit(wq->thread);
    wq->thread = NULL;
}

int http_wq_pool_init(void)
{
    int i;

#ifdef __linux__
    #include <sys/sysinfo.h>
    int cpus = get_nprocs_conf();
#else
    int cpus = HTTP_CLIENT_WORK_QUEUE_SIZE;
#endif // __linux__

    if (cpus <= 0) {
        HTTP_LOG_E("get nprocs conf failed!\n");
        return -1;
    }
    
    http_wq_t *wq = platform_memory_alloc(cpus * sizeof(http_wq_t));

    HTTP_ROBUSTNESS_CHECK(wq, HTTP_NULL_VALUE_ERROR);

    _wq_pool.wq = wq;
    for (i = 0; i < cpus; ++i, ++wq) {
        if (0 != _http_wq_init(wq)) {
            printf("http work queue init failed!\n");
            return -1;
        }
    }

    _wq_pool.cpus = cpus;
    _wq_pool.ring = 0;

    RETURN_ERROR(HTTP_SUCCESS_ERROR);
}

void http_wq_pool_deinit(void)
{
    int i;
    http_wq_t *wq = _wq_pool.wq;
    HTTP_ROBUSTNESS_CHECK(wq, HTTP_VOID)

    for (i = 0; i < _wq_pool.cpus; ++i, ++wq) {
        _http_wq_deinit(wq);
    }
    platform_memory_free(_wq_pool.wq);
}

int http_wq_add_task(http_worker_func_t func, void *data, size_t len)
{
    HTTP_ROBUSTNESS_CHECK(func, HTTP_MEM_NOT_ENOUGH_ERROR);

    int i = ++_wq_pool.ring % _wq_pool.cpus;
    http_wq_t *wq = _wq_pool.wq + i;
    
    _http_wq_worker_create(wq, func, data, len);

    RETURN_ERROR(HTTP_SUCCESS_ERROR);
}

void http_wq_wait_exit(void)
{
    int i;
    http_wq_t *wq = _wq_pool.wq;
    HTTP_ROBUSTNESS_CHECK(wq, HTTP_VOID)

    for (i = 0; i < _wq_pool.cpus; ++i, ++wq) {
        _http_wq_wait(wq);
    }
}

#endif // HTTP_USING_WORK_QUEUE
