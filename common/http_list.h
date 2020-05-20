/*
 * @Author: jiejie
 * @Github: https://github.com/jiejieTop
 * @Date: 2019-12-11 22:47:55
 * @LastEditTime: 2020-05-20 20:48:44
 * @Description: the code belongs to jiejie, please keep the author information and source code according to the license.
 */
#ifndef _HTTP_LIST_H_
#define _HTTP_LIST_H_

typedef struct http_list_node {
    struct http_list_node *next;
    struct http_list_node *prev;
} http_list_t;

#define OFFSET_OF_FIELD(type, field) \
    ((size_t)&(((type *)0)->field))

#define CONTAINER_OF_FIELD(ptr, type, field) \
    ((type *)((unsigned char *)(ptr) - OFFSET_OF_FIELD(type, field)))

#define HTTP_LIST_NODE(node) \
    { &(node), &(node) }

#define HTTP_LIST_DEFINE(list) \
    http_list_t list = { &(list), &(list) }

#define HTTP_LIST_ENTRY(list, type, field) \
    CONTAINER_OF_FIELD(list, type, field)

#define HTTP_LIST_FIRST_ENTRY(list, type, field) \
    HTTP_LIST_ENTRY((list)->next, type, field)

#define HTTP_LIST_FIRST_ENTRY_OR_NULL(list, type, field) \
    (http_list_is_empty(list) ? NULL : HTTP_LIST_FIRST_ENTRY(list, type, field))

#define HTTP_LIST_FOR_EACH(curr, list) \
    for (curr = (list)->next; curr != (list); curr = curr->next)

#define HTTP_LIST_FOR_EACH_PREV(curr, list) \
    for (curr = (list)->prev; curr != (list); curr = curr->prev)

#define HTTP_LIST_FOR_EACH_SAFE(curr, next, list) \
    for (curr = (list)->next, next = curr->next; curr != (list); \
            curr = next, next = curr->next)

#define HTTP_LIST_FOR_EACH_PREV_SAFE(curr, next, list) \
    for (curr = (list)->prev, next = curr->prev; \
            curr != (list); \
            curr = next, next = curr->prev)

void http_list_init(http_list_t *list);
void http_list_add(http_list_t *node, http_list_t *list);
void http_list_add_tail(http_list_t *node, http_list_t *list);
void http_list_del(http_list_t *entry);
void http_list_del_init(http_list_t *entry);
void http_list_move(http_list_t *node, http_list_t *list);
void http_list_move_tail(http_list_t *node, http_list_t *list);
int http_list_is_empty(http_list_t *list);

#endif /* _LIST_H_ */

