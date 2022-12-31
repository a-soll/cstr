#ifndef CS_H
#define CS_H

#include <stdbool.h>
#include <sys/types.h>

typedef struct _cs {
    char *string;
    size_t len;   // string length - not allocated size
    size_t alloc; // allocated size - not str length
} _cs;

typedef _cs *cstr;

// inits cstr with the given string
cstr cstrInit(const char *from);
// inits cstr with the given string and the given size.
// string can be empty.
cstr cstrInitSize(const char *from, size_t size);
void cstrDealloc(cstr cs);
void cstrCat(cstr to, const char *from);
// reallocs the cstr to the specified size
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
void cstrUpdateString(cstr str, const char *from);
/**
 * format and concat a string to given cstr
 * %s for string
 * %d for int
 *
 * ex:
 * cstrCatFmt(test, "%s %d", "ing", 1) -> testing 1
 */
void cstrCatFmt(cstr s, const char *fmt, ...);
/**
 * replaces substring in s->string with specified string
 *
 * ex (assume passing cstr with the string value)
 * cstrReplace(aaa<>bbb, <test>) -> aaa<test>bbb
 */
void cstrReplace(cstr s, const char *repl, const char *with);
/**
 * replaces substring between the two specified strings.
 * whole_match flag specifies whether to search for only
 * whole word matches (will replace in the middle of a string if false)
 * 
 * the passed cstr is deallocated - you need to assign it (or a new one)
 * to the return value. returns original cstr if no valid matches found.
 *
 * ex (assume passing cstr with the string value)
 * cstrReplaceBetween(aaa{bbb}ccc, {, }, eee, false) -> aaaeeeccc
 * cstrReplaceBetween(this is a test string, is, test, <replaced>, true) -> th<replaced> string
 * cstrReplaceBetween(this is a test string, is, test, <replaced>, false) -> this <replaced> string
 */
cstr cstrReplaceBetween(cstr str, const char *start, const char *end, const char *with, bool whole_match);
/**
 * concat string to cstr up to specified index
 *
 * ex (assume passing cstr with the string value)
 * cstrnCat(test, long string value, 11) -> test long string
 */
void cstrnCat(cstr str, const char *from, int end);
#endif /* CS_H */
