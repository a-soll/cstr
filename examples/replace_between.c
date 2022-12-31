#include "examples.h"

/**
 * this function replaces the text between (and including) the specified
 * start and end points. the same string can be used, so start=$ and end=$ will
 * replace between the first and next occurrance of $.
 * the whole_match flag can be used to specify whether or not to look for complete
 * string matches. if false, will replace on the first substring match regardless
 */
int main() {
    cstr s = cstrInit("this $replace here$ :)");
    // important to assign the cstr (or a new one) to the return value.
    // the original is deallocated.
    s = cstrReplaceBetween(s, "$", "$", "has been replaced", false);

    cstr s2 = cstrInit("this is a test string for replace");
    s2 = cstrReplaceBetween(s2, "is", "for", "<replaced>", false);

    cstr s3 = cstrInit("this is a test string for replace");
    s3 = cstrReplaceBetween(s3, "is", "for", "<replaced>", true);

    printf("%s\n", s->string);
    printf("%s\n", s2->string);
    printf("%s\n", s3->string);
    cstrDealloc(s);
    cstrDealloc(s2);
}

/**
 * output:
 * $ clang  replace_between.c ../cstr.c && ./a.out
 * this has been replaced :)
 * th<replaced> replace
 * this <replaced> replace
 */
