/*
 * @Author: jiejie
 * @Github: https://github.com/jiejieTop
 * @Date: 2020-05-08 21:08:39
 * @LastEditTime: 2020-05-11 11:34:10
 * @Description: the code belongs to jiejie, please keep the author information and source code according to the license.
 */
#ifndef _HTTP_GENERAL_H_
#define _HTTP_GENERAL_H_

#include <stdint.h>


#define DEFAULT_HTTP_PORT   "80"
#define DEFAULT_HTTPS_PORT  "443"

#define HTTP_GET_CONNECT_PARAMS_DEFINE(name, type, res)                                     \
    type http_get_connect_params_##name(http_connect_params_t *conn) {                      \
        HTTP_ROBUSTNESS_CHECK((conn), res);                                                 \
        return conn->http_##name;                                                           \
    }

#define HTTP_SET_CONNECT_PARAMS_DEFINE(name, type, res)                                     \
    type http_set_connect_params_##name(http_connect_params_t *conn, type t, int l) {       \
        HTTP_ROBUSTNESS_CHECK((conn && t), res);                                            \
        http_utils_assign_string(&conn->http_##name, t, l);                                 \
        return conn->http_##name;                                                           \
    }

#define HTTP_GET_CONNECT_PARAMS_STATEMENT(name, type)                       \
    type http_get_connect_params_##name(http_connect_params_t *);

#define HTTP_SET_CONNECT_PARAMS_STATEMENT(name, type)                       \
    type http_set_connect_params_##name(http_connect_params_t *, type, int);

#define HTTP_SET_AND_GET_CONNECT_PARAMS_DEFINE(name, type, res)             \
    HTTP_SET_CONNECT_PARAMS_DEFINE(name, type, res)                         \
    HTTP_GET_CONNECT_PARAMS_DEFINE(name, type, res)

#define HTTP_SET_AND_GET_CONNECT_PARAMS_STATEMENT(name, type)               \
    HTTP_SET_CONNECT_PARAMS_STATEMENT(name, type)                           \
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

HTTP_SET_AND_GET_CONNECT_PARAMS_STATEMENT(url, const char*)
HTTP_SET_AND_GET_CONNECT_PARAMS_STATEMENT(scheme, const char*)
HTTP_SET_AND_GET_CONNECT_PARAMS_STATEMENT(host, const char*)
HTTP_SET_AND_GET_CONNECT_PARAMS_STATEMENT(user, const char*)
HTTP_SET_AND_GET_CONNECT_PARAMS_STATEMENT(password, const char*)
HTTP_SET_AND_GET_CONNECT_PARAMS_STATEMENT(path, const char*)
HTTP_SET_AND_GET_CONNECT_PARAMS_STATEMENT(query, const char*)
HTTP_SET_AND_GET_CONNECT_PARAMS_STATEMENT(farg, const char*)
HTTP_SET_AND_GET_CONNECT_PARAMS_STATEMENT(port, const char*)


http_connect_params_t *http_assign_connect_params(void);
void http_release_connect_params(http_connect_params_t *connect_params);


#endif // !_HTTP_GENERAL_H_
