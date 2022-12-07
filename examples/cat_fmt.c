#include "../cstr.h"
#include <stdio.h>

int main() {
    cstr s = cstrInit("first");

    cstrCatFmt(s, " and %s and %drd", "second", 3);
    printf("%s\n", s->string);
    cstrDealloc(s);
}

/**
 * output:
 * $ clang cat_fmt.c ../cstr.c && ./a.out
 * first and second and 3rd
 */
