/*
 * @Author: jiejie
 * @Github: https://github.com/jiejieTop
 * @Date: 2019-12-23 19:26:27
 * @LastEditTime: 2020-05-28 17:22:07
 * @Description: the code belongs to jiejie, please keep the author information and source code according to the license.
 */
#include "string.h"
#include "platform_thread.h"
#include "platform_memory.h"

platform_thread_t *platform_thread_init( const char *name,
                                        void (*entry)(void *),
                                        void * const param,
                                        unsigned int stack_size,
                                        unsigned int priority,
                                        unsigned int tick)
{
    int res;
    platform_thread_t *thread = NULL;
    void *(*thread_entry) (void *);

    thread_entry = (void *(*)(void*))entry;
    thread = platform_memory_alloc(sizeof(platform_thread_t));
    memset(thread, 0, sizeof(platform_thread_t));
    
    res = pthread_create(&thread->thread, NULL, thread_entry, param);
    if(res != 0) {
        platform_memory_free(thread);
        return NULL;
    }

    pthread_cond_init(&thread->cond, NULL);
    pthread_mutex_init(&thread->mutex, NULL);

    return thread;
}

void platform_thread_startup(platform_thread_t* thread)
{
    (void) thread;
}

void platform_thread_stop(platform_thread_t* thread)
{
    if (NULL != thread) {
        pthread_mutex_lock(&thread->mutex);
        pthread_cond_wait(&thread->cond, &thread->mutex);
        pthread_mutex_unlock(&thread->mutex);
    }
}

void platform_thread_start(platform_thread_t* thread)
{
    if (NULL != thread) {
        pthread_mutex_lock(&thread->mutex); 
        pthread_cond_signal(&thread->cond);
        pthread_mutex_unlock(&thread->mutex);
    }
}

void platform_thread_destroy(platform_thread_t* thread)
{
    if (NULL != thread) {
        pthread_detach(thread->thread);
        pthread_cond_destroy(&thread->cond);
        pthread_mutex_destroy(&thread->mutex);
    }
}

void platform_thread_notice_enter(platform_thread_t* thread)
{
    (void) thread;
}

void platform_thread_notice_exit(platform_thread_t* thread)
{
    (void) thread;
}

void platform_thread_wait_exit(platform_thread_t* thread)
{
    if (NULL != thread) {
        pthread_join(thread->thread, NULL);
        pthread_cond_destroy(&thread->cond);
        pthread_mutex_destroy(&thread->mutex);
    }   
}


