#include "examples.h"

/**
 * this function replaces the text between (and including) the specified
 * start and end points. the same string can be used, so start=$ and end=$ will
 * replace between the first and next occurrance of $.
 * the whole_match flag can be used to specify whether or not to look for complete
 * string matches. if false, will replace on the first substring match regardless.
 *
 * you need to use the return cstr as the original is deallocated.
 */
int main() {
    cstr s = cstrInit("aaa <> bbb");
    s = cstrReplaceBetween(s, "aaa", "bbb", " XXX ", true);

    cstr s2 = cstrInit("aaa<>bbb");
    s2 = cstrReplaceBetween(s2, "aaa", "bbb", "XXX", true);

    cstr s3 = cstrInit("aaa<>bbb");
    s3 = cstrReplaceBetween(s3, "aaa", "bbb", "XXX", false);

    printf("%s\n", s->string);
    printf("%s\n", s2->string);
    printf("%s\n", s3->string);
    cstrDealloc(s);
    cstrDealloc(s2);
    cstrDealloc(s3);
}

/**
 * output:
 * $ clang  replace_between.c ../cstr.c && ./a.out
 * aaa XXX bbb
 * aaa<>bbb
 * aaaXXXbbb
 */
