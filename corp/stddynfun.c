/* Copyright (c) 1999-2013  Pavel Rychly        */

#include <string.h>
#include <ctype.h>
#include <locale.h>

const char * getnchar (const char* x, int n)
{
    static char ch[2] = " ";
    if ((int)strlen (x) < n)
        ch[0] = '\0';
    else
        ch[0] = x[n-1];
    return ch;
}

const char * getnextchar (const char* x, const char attr)
{
    static char ch[3] = "  ";
    ch[0] = '\0';
    while (*x) {
        if (*x++ == attr) {
            ch[0] = attr;
            ch[1] = *x;
            break;
        }
    }
    return ch;
}

const char * getnextchars (const char* x, const char attr, int len)
{
    static char ch[11] = " ";
    char *s;
    if (len > 10)
        len = 10;
    while (*x) {
        if (*x++ == attr) {
            ch[0] = *x;
            break;
        }
    }
    for (s=ch; *x && len; len--)
        *s++ = *x++;
    *s = 0;
    return ch;
}

const char * getfirstn (const char* x, int len)
{
    static size_t ressize = 32;
    static char *result = (char *) malloc(ressize);
    if (len < 0)
        len = 0;
    if (ressize <= len) {
        ressize = len + 1;
        result = (char *) realloc (result, ressize);
    }
    strncpy (result, x, len);
    result[len] = '\0';
    return result;
}

const char * getfirstbysep (const char* x, const char sep)
{
    const char *p = strchr (x, sep);
    if (!p) return x;
    return getfirstn (x, p - x);
}

const char * getlastn (const char* x, int len)
{
    size_t x_len = strlen(x);
    if (x_len <= len)
        return x;
    static size_t ressize = 32;
    static char *result = (char *) malloc(ressize);
    if (len < 0)
        len = 0;
    if (ressize <= len) {
        ressize = len + 1;
        result = (char *) realloc (result, ressize);
    }
    strcpy (result, x + x_len - len);
    result[len] = '\0';
    return result;
}

const char * utf8getlastn (const char* x, int len)
{
    return utf8suffix (x, len);
}

const char * striplastn (const char* x, int cnt)
{
    return getfirstn (x, strlen (x) - cnt);
}

const char * lowercase (const char* x, const char *locale)
{
    static char *result = NULL;
    static size_t ressize = 0;
    size_t len = strlen (x);
    if (ressize <= len) {
        ressize = len + 1;
        result = (char *) realloc (result, ressize);
    }
    char *r;
    const char *prev_locale = setlocale (LC_CTYPE, locale);
    for (r = result; *x; x++, r++)
        *r = tolower (*x);
    setlocale (LC_CTYPE, prev_locale);
    *r = '\0';
    return result;
}

const char * url2domain (const char* url, int level)
{
    static char *result = NULL;
    static size_t ressize = 0;
    if (!strncmp(url, "http://", 7))
        url += 7;
    if (!strncmp(url, "www.", 4))
        url += 4;
    const char *end = url;
    while (*end && *end != '/')
        end++;
    const char *beg = (level ? end : url);
    while (level--) {
        if (beg != url)
            beg--;
        while (beg != url && *beg != '.')
            beg--;
    }
    if (beg != url)
        beg++;
    size_t len = end - beg;
    if (ressize <= len) {
        ressize = len + 1;
        result = (char *) realloc (result, ressize);
    }
    strncpy (result, beg, len);
    result[len] = '\0';
    return result;
}

// vim: ts=4 sw=4 sta et sts=4 si cindent tw=80:
