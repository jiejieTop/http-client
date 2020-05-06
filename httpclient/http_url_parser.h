/*
 * @Author: jiejie
 * @Github: https://github.com/jiejieTop
 * @Date: 2020-05-03 20:39:06
 * @LastEditTime: 2020-05-06 09:07:33
 * @Description: the code belongs to jiejie, please keep the author information and source code according to the license.
 */
#ifndef _HTTP_URL_PARSER_H_
#define _HTTP_URL_PARSER_H_

#include <stdint.h>

#define DEFAULT_HTTP_PORT   (80)
#define DEFAULT_HTTPS_PORT  (443)

typedef struct http_connect_params {
    char                        *http_url;
    char                        *http_scheme;
    char                        *http_host;
    char                        *http_username;
    char                        *http_password;
    char                        *http_path;
    char                        *http_query;
    char                        *http_farg;
    char                        *http_cert_pem;
    uint16_t                    http_port;
} http_connect_params_t;


int http_url_parsing(http_connect_params_t *connect_params, const char *url);





#endif // !_HTTP_URL_PARSER_H_
