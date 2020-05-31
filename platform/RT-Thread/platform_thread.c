/*
 * @Author: jiejie
 * @Github: https://github.com/jiejieTop
 * @Date: 2019-12-23 19:26:27
 * @LastEditTime: 2020-05-28 17:21:39
 * @Description: the code belongs to jiejie, please keep the author information and source code according to the license.
 */
#include "platform_thread.h"
#include "platform_memory.h"

platform_thread_t *platform_thread_init( const char *name,
                                        void (*entry)(void *),
                                        void * const param,
                                        unsigned int stack_size,
                                        unsigned int priority,
                                        unsigned int tick)
{
    rt_err_t err;
    platform_thread_t *thread;
    uint8_t *thread_stack;

    thread = platform_memory_alloc(sizeof(platform_thread_t));

    thread_stack = (uint8_t*) platform_memory_alloc(stack_size);

    err =  rt_thread_init(&(thread->thread),
                            (const char *)name,
                                         entry,
                                         param,
                                         thread_stack,
                                         stack_size,
                                         priority,
                                         tick);
    
    thread->sem = rt_sem_create(name, 1, RT_IPC_FLAG_PRIO);

    if ((err != RT_EOK) || (NULL == thread->sem)) {
        platform_memory_free(thread);
        platform_memory_free(thread_stack);
        return NULL;
    }

    return thread;
}

void platform_thread_startup(platform_thread_t* thread)
{
    rt_thread_startup(&(thread->thread));
}


void platform_thread_stop(platform_thread_t* thread)
{
    rt_thread_suspend(&(thread->thread));
    rt_schedule();
}

void platform_thread_start(platform_thread_t* thread)
{
    rt_thread_resume(&(thread->thread));
}

void platform_thread_destroy(platform_thread_t* thread)
{
    if (NULL == thread) 
        return;
    
    rt_sem_delete(thread->sem);
    rt_thread_detach(&(thread->thread));
    
    platform_memory_free(&(thread->thread));
    platform_memory_free(&(thread->thread.stack_addr));
}

void platform_thread_notice_enter(platform_thread_t* thread)
{
    rt_sem_take(thread->sem, RT_WAITING_FOREVER);
}

void platform_thread_notice_exit(platform_thread_t* thread)
{
    rt_sem_release(thread->sem);
}

void platform_thread_wait_exit(platform_thread_t* thread)
{
    rt_sem_take(thread->sem, RT_WAITING_FOREVER);
    platform_thread_destroy(thread);
}
