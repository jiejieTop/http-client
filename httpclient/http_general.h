/*
 * @Author: jiejie
 * @Github: https://github.com/jiejieTop
 * @Date: 2020-05-08 21:08:39
 * @LastEditTime: 2020-05-08 21:20:46
 * @Description: the code belongs to jiejie, please keep the author information and source code according to the license.
 */
#ifndef _HTTP_GENERAL_H_
#define _HTTP_GENERAL_H_

#include <stdint.h>


#define DEFAULT_HTTP_PORT   "80"
#define DEFAULT_HTTPS_PORT  "443"

#define HTTP_GET_CONNECT_PARAMS_DEFINE(name, type, res)                                     \
    type http_get_connect_params_##name(http_connect_params_t *connect_params) {            \
        HTTP_ROBUSTNESS_CHECK((connect_params), res);                                       \
        return connect_params->http_##name;                                                 \
    }

#define HTTP_SET_CONNECT_PARAMS_DEFINE(name, type, res)                                     \
    type http_set_connect_params_##name(http_connect_params_t *connect_params, type t) {    \
        HTTP_ROBUSTNESS_CHECK((connect_params && t), res);                                  \
        connect_params->http_##name = t;                                                    \
        return connect_params->http_##name;                                                 \
    }

#define HTTP_GET_CONNECT_PARAMS_STATEMENT(name, type)                   \
    type http_get_connect_params_##name(http_connect_params_t *);

#define HTTP_SET_CONNECT_PARAMS_STATEMENT(name, type)                   \
    type http_set_connect_params_##name(http_connect_params_t *, type);

#define HTTP_SET_AND_GET_CONNECT_PARAMS_DEFINE(name, type, res)         \
    HTTP_SET_CONNECT_PARAMS_DEFINE(name, type, res)                     \
    HTTP_GET_CONNECT_PARAMS_DEFINE(name, type, res)

#define HTTP_SET_AND_GET_CONNECT_PARAMS_STATEMENT(name, type)           \
    HTTP_SET_CONNECT_PARAMS_STATEMENT(name, type)                       \
    HTTP_GET_CONNECT_PARAMS_STATEMENT(name, type)


typedef struct http_connect_params {
    char                        *http_url;
    char                        *http_scheme;
    char                        *http_host;
    char                        *http_user;
    char                        *http_password;
    char                        *http_path;
    char                        *http_query;
    char                        *http_farg;
    char                        *http_cert_pem;
    char                        *http_port;
} http_connect_params_t;

HTTP_SET_AND_GET_CONNECT_PARAMS_STATEMENT(url, char*)
HTTP_SET_AND_GET_CONNECT_PARAMS_STATEMENT(scheme, char*)
HTTP_SET_AND_GET_CONNECT_PARAMS_STATEMENT(host, char*)
HTTP_SET_AND_GET_CONNECT_PARAMS_STATEMENT(user, char*)
HTTP_SET_AND_GET_CONNECT_PARAMS_STATEMENT(password, char*)
HTTP_SET_AND_GET_CONNECT_PARAMS_STATEMENT(path, char*)
HTTP_SET_AND_GET_CONNECT_PARAMS_STATEMENT(query, char*)
HTTP_SET_AND_GET_CONNECT_PARAMS_STATEMENT(farg, char*)
HTTP_SET_AND_GET_CONNECT_PARAMS_STATEMENT(port, char*)


http_connect_params_t *http_assign_connect_params(void);
void http_release_connect_params(http_connect_params_t *connect_params);


#endif // !_HTTP_GENERAL_H_
