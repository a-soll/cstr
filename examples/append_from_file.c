/**
 * compile with:
 * clang append_from_file.c ../cstr.c
 */

#include "../cstr.h"
#include <stdbool.h>
#include <stdio.h>

#define BUFF_SIZE 500

int main() {
    char buf[BUFF_SIZE];
    FILE *f;
    cstr str;
    bool first = true;

    if ((f = fopen("large_string.txt", "r"))) {
        while (fgets(buf, BUFF_SIZE, f)) {
            if (first) {             // need to initialize cstr but only once
                str = cstrInit(buf); // returns initialized cstr with string set to first buffer
                first = false;
            } else {
                cstrCat(str, buf);
            }
        }
    }

    printf("%s\n", str->string);
    printf("%zu\n", str->len);
    printf("%zu\n", str->alloc);

    cstrDealloc(str);
}
