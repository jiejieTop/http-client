/*
 * @Author: jiejie
 * @Github: https://github.com/jiejieTop
 * @Date: 2019-12-14 22:02:07
 * @LastEditTime: 2020-05-06 16:36:48
 * @Description: the code belongs to jiejie, please keep the author information and source code according to the license.
 */
#include "platform_memory.h"

void *platform_memory_alloc(size_t size)
{
    return malloc(size);
}

void *platform_memory_calloc(size_t num, size_t size)
{
    return calloc(num, size);
}

void *platform_memory_realloc(void *ptr, size_t size)
{
    if (NULL == ptr) 
        return malloc(size);
    else 
        return realloc(ptr, size);
}

void platform_memory_free(void *ptr)
{
    free(ptr);
}



