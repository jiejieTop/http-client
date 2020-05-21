/*
 * @Author: jiejie
 * @Github: https://github.com/jiejieTop
 * @Date: 2020-05-03 13:35:08
 * @LastEditTime: 2020-05-21 21:33:35
 * @Description: the code belongs to jiejie, please keep the author information and source code according to the license.
 */

#include <http_utils.h>
#include <platform_memory.h>



int _http_utils_isspace(int x)
{
    if(x==' '||x=='\t'||x=='\n'||x=='\f'||x=='\b'||x=='\r')
        return 1;
    else  
        return 0;
}

int _http_utils_isdigit(int x)
{
    if(x<='9'&&x>='0')         
        return 1; 
    else 
        return 0;
}

static int _http_utils_to_lower(int c)
{
    if ((c >= 'A') && (c <= 'Z'))
        return c + ('a' - 'A');
    return c;
}

size_t http_utils_count_concat(va_list *ap)
{
    size_t total = 0;
    char *next;

    while ((next = va_arg(*ap, char *)) != NULL)
    total += strlen(next);

    return total;
}

void http_utils_concat(char *str, va_list *ap) 
{
    char *next;

    while ((next = va_arg(*ap, char *)) != NULL) {
#ifdef HAVE_STPCPY
        str = stpcpy(str, next);
#else
	size_t len = strlen(next);
	memcpy(str, next, len);
	str += len;
#endif
    }
}

int http_utils_match(const char *str1, const char *str2)
{
    int c1, c2;
    do {
        c1 = *str1++;
        c2 = *str2++;
    } while((c1 == c2) && c1);

    return c1 - c2;
}

int http_utils_nmatch(const char *str1, const char *str2, int n)
{
    int i = 0;
    int c1, c2;
    do {
        c1 = *str1++;
        c2 = *str2++;
        i++;
    } while(((c1 == c2) && c1) && (i < n));

    return c1 - c2;
}

int http_utils_ignore_case_match(const char *str1, const char *str2)
{
    int c1, c2;

    do {
        c1 = _http_utils_to_lower(*str1++);
        c2 = _http_utils_to_lower(*str2++);
    } while((c1 == c2) && c1);

    return c1 - c2;
}

int http_utils_ignore_case_nmatch(const char *str1, const char *str2, int n)
{
    int i = 0;
    int c1, c2;

    do {
        c1 = _http_utils_to_lower(*str1++);
        c2 = _http_utils_to_lower(*str2++);
        i++;
    } while(((c1 == c2) && c1) && (i < n));

    return c1 - c2;
}

char *http_utils_right_strchr(char * s, char x)
{
    int i = strlen(s);
    if (!(*s)) 
        return 0;
    while (s[i-1]) {
        if (strchr(s + (i - 1), x)) {
            return (s + (i - 1));
        } else {
            i--;
        }
    }
    return 0;
}

char *http_utils_assign_string(char **str, const char *new_str, int len)
{
    int l = len;
    char *old_str = *str;

    if (new_str == NULL) {
        return NULL;
    }
    
    if (l <= 0) {
        l = strlen(new_str);
    }

    if (old_str) {
        old_str = platform_memory_realloc(old_str, l + 1);
    } else {
        old_str = platform_memory_alloc(l + 1);
    }

    if (NULL != old_str) {
        memcpy(old_str, new_str, l);
        *str = old_str;
        old_str[l] = '\0';
    }
    return old_str;
}

void http_utils_release_string(char *str)
{
    if (NULL != str)
        platform_memory_free(str);
}

int http_utils_atoi(const char *nptr)
{
    int c;              /* current char */
    int total;          /* current total */
    int sign;           /* if '-', then negative, otherwise positive */

    /* skip whitespace */
    while (_http_utils_isspace((int)(unsigned char)*nptr))
        ++nptr;

    c = (int)(unsigned char)*nptr++;
    sign = c;                       /* save sign indication */
    
    if (c == '-' || c == '+')
        c = (int)(unsigned char)*nptr++;    /* skip sign */

    total = 0;

    while (_http_utils_isdigit(c)) {
        total = 10 * total + (c - '0');     /* accumulate digit */
        c = (int)(unsigned char)*nptr++;    /* get next char */
    }

    if (sign == '-')
        return -total;
    else
        return total;   /* return result, negated if necessary */
}
 

char *http_utils_itoa(int value, char *string, int radix)
{
    char tmp[33];
    char* tp = tmp;
    int i;
    unsigned v;
    int sign;
    char* sp;
    
    if (radix > 36 || radix <= 1)
        return 0;

    sign = (radix == 10 && value < 0);

    if (sign)
        v = -value;
    
    else
        v = (unsigned)value;
    
    while (v || tp == tmp) {
        i = v % radix;
        v = v / radix;
        
        if (i < 10)
            *tp++ = i + '0';
        else
            *tp++ = i + 'a' - 10;
    }

    if (string == 0)
        string = (char*)platform_memory_alloc((tp - tmp) + sign + 1);
    
    sp = string;

    if (sign)
        *sp++ = '-';

    while (tp > tmp)
        *sp++ = *--tp;
    *sp = 0;

    return string;
}








