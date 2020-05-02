/*
 * @Author: jiejie
 * @Github: https://github.com/jiejieTop
 * @Date: 2019-12-11 22:46:33
 * @LastEditTime: 2020-04-27 23:28:12
 * @Description: the following code references TencentOS tiny, please keep the author information and source code according to the license.
 */

# include "http_list.h"

static void _http_list_add(http_list_t *node, http_list_t *prev, http_list_t *next)
{
    next->prev = node;
    node->next = next;
    node->prev = prev;
    prev->next = node;
}

static void _http_list_del(http_list_t *prev, http_list_t *next)
{
    next->prev = prev;
    prev->next = next;
}

static void _http_list_del_entry(http_list_t *entry)
{
    _http_list_del(entry->prev, entry->next);
}

void http_list_init(http_list_t *list)
{
    list->next = list;
    list->prev = list;
}

void http_list_add(http_list_t *node, http_list_t *list)
{
    _http_list_add(node, list, list->next);
}

void http_list_add_tail(http_list_t *node, http_list_t *list)
{
    _http_list_add(node, list->prev, list);
}

void http_list_del(http_list_t *entry)
{
    _http_list_del(entry->prev, entry->next);
}

void http_list_del_init(http_list_t *entry)
{
    _http_list_del_entry(entry);
    http_list_init(entry);
}

void http_list_move(http_list_t *node, http_list_t *list)
{
    _http_list_del_entry(node);
    http_list_add(node, list);
}

void http_list_move_tail(http_list_t *node, http_list_t *list)
{
    _http_list_del_entry(node);
    http_list_add_tail(node, list);
}

int http_list_is_empty(http_list_t *list)
{
    return list->next == list;
}
