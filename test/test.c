/*
 * @Author: jiejie
 * @Github: https://github.com/jiejieTop
 * @Date: 2019-12-11 21:53:07
 * @LastEditTime: 2020-05-26 16:54:34
 * @Description: the code belongs to jiejie, please keep the author information and source code according to the license.
 */
#include <stdio.h>
#include <unistd.h>
#include <httpclient.h>

extern const char *ca_get();
extern void http_url_parsing_test(void);
extern void http_get_test(void);
extern void http_post_test(void);
extern void http_redirect_test(void);
extern void http_get_file_test(void);

int main(void)
{
    http_log_init();

    http_client_init(ca_get());

    http_url_parsing_test();

    http_get_test();

    http_post_test();

    http_redirect_test();

    http_get_file_test();

    http_client_exit();

    // sleep(50);
    
    return 0;
}