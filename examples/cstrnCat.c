#include "examples.h"

int main() {
    cstr s = cstrInit("testing a");
    printf("len: %lu alloc: %lu\n", s->len, s->alloc);

    cstrnCat(s, " long string value", 12);

    printf("len: %lu alloc: %lu\n", s->len, s->alloc);
    printf("%s\n", s->string);
}

/**
 * output:
 * $ clang  cstrnCat.c ../cstr.c && ./a.out
 * len: 9 alloc: 10
 * len: 21 alloc: 40
 * testing a long string
 */
