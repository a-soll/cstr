#include "../cstr.h"
#include <stdio.h>

int main() {
    cstr str;

    str = cstrInit("test,this,string");

    int count = 0;
    cstr *split = cstrSplit(str, ',', &count);
    printf("%d\n", count);

    int i = 0;
    while (split[i] != NULL) {
        printf("%s\n", split[i]->string);
        i++;
    }
    cstrDealloc(str);
    cstrArrayDealloc(split);
}

/*
output:
$ clang split.c ../cstr.c -o a.out && ./a.out
3
test
this
string
*/
