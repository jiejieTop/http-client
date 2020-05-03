/*
 * @Author: jiejie
 * @Github: https://github.com/jiejieTop
 * @Date: 2020-05-03 13:35:08
 * @LastEditTime: 2020-05-03 18:36:15
 * @Description: the code belongs to jiejie, please keep the author information and source code according to the license.
 */

#include <http_utils.h>
#include <platform_memory.h>
#include <string.h>
#include <http_log.h>

char *http_utils_assign_string(char **str, const char *new_str, int len)
{
    int l = len;
    char *old_str = *str;

    if (new_str == NULL) {
        return NULL;
    }
    
    if (l <= 0) {
        l = strlen(new_str);
    }

    if (old_str) {
        platform_memory_free(old_str);
        old_str = NULL;
    } 
    
    old_str = platform_memory_alloc(l + 1);

    if (NULL != old_str) {
        memcpy(old_str, new_str, l);
        *str = old_str;
    }
    return old_str;
}

void http_utils_release_string(char *str)
{
    if (NULL != str)
        platform_memory_free(str);
}

