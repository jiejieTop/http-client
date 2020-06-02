/*
 * @Author: jiejie
 * @Github: https://github.com/jiejieTop
 * @Date: 2020-05-08 21:08:39
 * @LastEditTime: 2020-06-02 22:17:06
 * @Description: the code belongs to jiejie, please keep the author information and source code according to the license.
 */
#ifndef _HTTP_GENERAL_H_
#define _HTTP_GENERAL_H_

#include <stdint.h>


#define DEFAULT_HTTP_PORT   "80"
#define DEFAULT_HTTPS_PORT  "443"

#define HTTP_GENERAL_FLAG                                   \
    union {                                                 \
        uint32_t                        all_flag;           \
        struct {                                            \
            uint32_t again              : 4;                \
            uint32_t redirects          : 4;                \
            uint32_t retry              : 4;                \
            uint32_t authenticate       : 4;                \
            uint32_t chunked            : 4;                \
            uint32_t chunked_complete   : 4;                \
            uint32_t complete           : 4;                \
            uint32_t keep_alive         : 4;                \
        } flag_t;                                           \
    } flag;                                                 

typedef enum {
    http_keep_alive_flag   =   0x0001 << 0
} http_general_flag_t;

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

#define HTTP_FREE_CONNECT_PARAMS(conn, name)                                \
    do {                                                                    \
        if ( NULL != ( conn->http_##name ) ) {                              \
            platform_memory_free( conn->http_##name );                      \
            conn->http_##name = NULL;                                       \
        }                                                                   \
    } while(0)

#define HTTP_FREE_ALL_CONNECT_PARAMS(conn)                                  \
    do {                                                                    \
        HTTP_FREE_CONNECT_PARAMS(conn, url);                                \
        HTTP_FREE_CONNECT_PARAMS(conn, scheme);                             \
        HTTP_FREE_CONNECT_PARAMS(conn, host);                               \
        HTTP_FREE_CONNECT_PARAMS(conn, user);                               \
        HTTP_FREE_CONNECT_PARAMS(conn, password);                           \
        HTTP_FREE_CONNECT_PARAMS(conn, path);                               \
        HTTP_FREE_CONNECT_PARAMS(conn, query);                              \
        HTTP_FREE_CONNECT_PARAMS(conn, farg);                               \
        HTTP_FREE_CONNECT_PARAMS(conn, cert_pem);                           \
        HTTP_FREE_CONNECT_PARAMS(conn, port);                               \
    } while(0)

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
void http_release_connect_params_variables(http_connect_params_t *connect_params);

#endif // !_HTTP_GENERAL_H_
