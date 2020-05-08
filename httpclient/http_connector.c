/*
 * @Author: jiejie
 * @Github: https://github.com/jiejieTop
 * @Date: 2020-04-16 20:36:06
 * @LastEditTime: 2020-05-08 21:11:54
 * @Description: the code belongs to jiejie, please keep the author information and source code according to the license.
 */

#include <http_url_parser.h>
#include <http_connector.h>
#include <http_log.h>
#include <http_error.h>
#include <platform_memory.h>

http_list_t _http_idle_list;
http_list_t _http_lease_list;
http_list_t _http_stable_list;
http_list_t _http_pending_list;










