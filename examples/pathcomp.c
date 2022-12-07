#include "../cstr.h"
#include <stdio.h>

int main() {
    cstr s = cstrInit("aa.bb=cc.dd");

    cstr a = pathcomp(s, ".LR");
    cstr b = pathcomp(s, ".LR.RR");
    cstr c = pathcomp(s, ".LR.RL");
    cstr d = pathcomp(s, ".LR=RL");
    cstr e = pathcomp(s, ".LR.RL=RL");

    printf("%s\n", a->string);
    printf("%s\n", b->string);
    printf("%s\n", c->string);
    printf("%s\n", d->string);
    printf("%s\n", e->string);

    cstrDealloc(s);
    cstrDealloc(a);
    cstrDealloc(b);
    cstrDealloc(c);
    cstrDealloc(d);
    cstrDealloc(e);
}

/**
 * output:
 * $ clang pathcomp.c ../cstr.c -o a.out && ./a.out
 * bb=cc.dd
 * dd
 * bb=cc
 * bb
 * bb
 */
