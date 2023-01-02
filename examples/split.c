#include "examples.h"

int main() {
    cstr str;
    str = cstrInit("test<>this<>string");

    int count = 0;
    cstr *split = cstrSplit(str, "<>", &count);
    printf("count %d\n", count);

    for (int i = 0; i < count; i++) {
        printf("%s\n", split[i]->string);
    }

    // setting count to 1 before passing will only allow for 1 split. can be any number.
    count = 1;
    cstr *split2 = cstrSplit(str, "<>", &count);
    printf("count %d\n", count);

    for (int i = 0; i < count; i++) {
        printf("%s\n", split2[i]->string);
    }

    cstrDealloc(str);
    cstrArrayDealloc(split);
    cstrArrayDealloc(split2);
}

/*
output:
$ clang split.c ../cstr.c && ./a.out
count 3
test
this
string
count 2
test
this<>string
*/
