#include "cstr.h"
#include <stdbool.h>
#include <stdio.h>

cstr cstrInit(char *from) {
    size_t from_len = strlen(from);
    cstr ret = (cstr)malloc(sizeof(_cs));
    ret->len = from_len;
    ret->string = malloc(sizeof(char) * from_len);
    ret->alloc = from_len;
    memcpy(ret->string, from, from_len);
    ret->string[from_len] = '\0';
    return ret;
}

void makeRoomFor(cstr s, size_t len) {
    if (s->alloc <= len) {
        while (s->alloc <= len) {
            s->alloc *= 2;
        }
        s->string = realloc(s->string, s->alloc);
    }
}

void cstrGrow(cstr s, size_t len) {
    size_t cur_alloc = s->alloc;
    if (len > cur_alloc) {
        s->string = realloc(s->string, len);
        s->alloc = len;
        memset(s->string + s->len, 0, (s->alloc - cur_alloc));
    }
}

void cstrCat(cstr to, const char *from) {
    size_t cur_len = to->len;
    size_t from_len = strlen(from);
    size_t needs = from_len + cur_len;
    makeRoomFor(to, needs);
    memcpy(to->string + cur_len, from, from_len);
    to->len = needs;
    to->string[to->len] = '\0';
}

void cstrDealloc(cstr cs) {
    free(cs->string);
    free(cs);
}
