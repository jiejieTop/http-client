/*
 * @Author: jiejie
 * @Github: https://github.com/jiejieTop
 * @Date: 2020-05-03 13:37:23
 * @LastEditTime: 2020-05-03 21:02:52
 * @Description: the code belongs to jiejie, please keep the author information and source code according to the license.
 */

#ifndef _HTTP_UTILS_H_
#define _HTTP_UTILS_H_

#define HTTP_ROBUSTNESS_CHECK(item, err) if (!(item)) {                                         \
        HTTP_LOG_E("%s:%d %s()... check for error.", __FILE__, __LINE__, __FUNCTION__);         \
        return err; }

char *http_utils_assign_string(char **str, const char *new_str, int len);
void http_utils_release_string(char *str);




#endif // !_HTTP_UTILS_H_

