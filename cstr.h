#ifndef CS_H
#define CS_H

#include <stdarg.h>
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
cstr *cstrSplit(cstr str, char split_on, int *count);
/**
 * search for character and return string from there.
 * expr is the char to search for [0], the direction to search from [1],
 * and direction to add to string [2]. can include multiple patterns.
 *
 * ex (assume passing cstr with these strings):
 * pathcomp(a.b.c, .LL) -> a
 * pathcomp(a.b.c, .LR) -> b.c
 * pathcomp(a.b.c, .RR) -> c
 * pathcomp(a.b.c, .RL) -> a.b
 * pathcomp(a.b.c, .LR.RL) -> b
 */
cstr pathcomp(cstr s, const char *expr);
void cstrArrayDealloc(cstr *arr);
// update cstr->string to new value (from)
void cstrUpdateString(cstr str, char *from);
/**
 * format a cstr
 * %s for string
 * %d for int
 */
void cstrCatFmt(cstr s, char const *fmt, ...);

#endif /* CS_H */
