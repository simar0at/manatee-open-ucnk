#ifndef UTF8_HH
#define UTF8_HH

#include <stdint.h>

unsigned int utf8len (const char *bytes);
int32_t utf8char (const char *bytes, int n);
const unsigned char * utf8_tolower (const unsigned char *src);
const char * utf8suffix (const char *bytes, int n);

#endif
