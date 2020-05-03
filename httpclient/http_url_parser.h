/*
 * @Author: jiejie
 * @Github: https://github.com/jiejieTop
 * @Date: 2020-05-03 20:39:06
 * @LastEditTime: 2020-05-03 23:00:19
 * @Description: the code belongs to jiejie, please keep the author information and source code according to the license.
 */
#ifndef _HTTP_URL_PARSER_H_
#define _HTTP_URL_PARSER_H_

#include <http_interceptor.h>

#define DEFAULT_HTTP_PORT   (80)
#define DEFAULT_HTTPS_PORT  (443)


int http_url_parsing(http_connect_params_t *connect_params, const char *url);





#endif // !_HTTP_URL_PARSER_H_
