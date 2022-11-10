#ifndef CS_H
#define CS_H

#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

typedef struct _cs {
    char *string;
    size_t len;   // string length - not allocated size
    size_t alloc; // allocated size - not str length
} _cs;

typedef _cs *cstr;

cstr cstrInit(char *from);
void cstrDealloc(cstr cs);
void cstrCat(cstr to, const char *from);
void cstrGrow(cstr s, size_t len);

#endif /* CS_H */
