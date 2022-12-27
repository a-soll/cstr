#include "../cstr.h"
#include <stdio.h>

int main() {
    cstr s = cstrInit("aaa<>bbb<>ccc");
    printf("%s\n", s->string);
    printf("%lu\n", strlen(s->string));
    printf("%lu\n", s->len);
    printf("%lu\n", s->alloc);

    cstrReplace(s, "<>", "<test>");
    printf("%s\n", s->string);
    printf("%lu\n", strlen(s->string));
    printf("%lu\n", s->len);
    printf("%lu\n", s->alloc);

    cstrDealloc(s);
}

/**
 * output:
 * $ clang replace.c ../cstr.c && a.out
 * aaa<>bbb<>ccc
 * 13
 * 13
 * 14
 * aaa<test>bbb<test>ccc
 * 21
 * 21
 * 28
 */
