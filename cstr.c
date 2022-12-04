#include "cstr.h"
#include <stdbool.h>
#include <stdio.h>

cstr cstrInit(char *from) {
    size_t from_len = strlen(from);
    cstr ret = (cstr)malloc(sizeof(_cs));
    ret->len = from_len;
    ret->string = malloc(sizeof(char) * from_len + 1);
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

void cstrArrayDealloc(cstr *arr) {
    int i = 0;
    while (arr[i] != NULL) {
        cstrDealloc(arr[i]);
        i++;
    }
    free(arr);
}

cstr *cstrSplit(cstr str, char split_on, int *count) {
    int ind = 0;
    int elements = 0;
    int slots = 2;
    char tmp[str->len];
    cstr *c = malloc(sizeof(cstr) * slots);

    for (int i = 0; i < str->len; i++) {
        if (str->string[i] == split_on) {
            if (slots <= elements) {
                slots *= 2;
                c = realloc(c, sizeof(cstr) * slots);
            }
            tmp[ind] = '\0';
            c[elements] = cstrInit(tmp);
            elements++;
            ind = 0;
        } else {
            tmp[ind] = str->string[i];
            ind++;
        }
    }
    if (slots == elements) { // resize to fit terminating NULL
        c = realloc(c, sizeof(cstr) * (slots + 1));
    }
    tmp[ind] = '\0';
    c[elements] = cstrInit(tmp);
    elements++;
    c[elements] = NULL;
    *count = elements;
    return c;
}

static int pathcompRight(cstr str, char *tmp, const char *expr) {
    int item_ind = -1;
    int tmp_ind = 0;

    for (int i = str->len; i >= 0; i--) {
        if (str->string[i] == expr[0]) {
            item_ind = i;
            break;
        }
    }

    if (expr[2] == 'R') {
        // check for valid char to right of searched char
        if (item_ind + 1 >= str->len) {
            return -1;
        }
        char *start = &str->string[item_ind + 1];
        char *end = &str->string[str->len];
        memcpy(tmp, start, end - start);
        int new_len = str->len - item_ind - 1;
        tmp[new_len] = '\0';
    } else if (expr[2] == 'L') {
        // check for valid char to left of searched char
        if (!str->string[item_ind - 1]) {
            return -1;
        }
        char *start = &str->string[0];
        char *end = &str->string[item_ind];
        memcpy(tmp, start, end - start);
    } else { // invalid pattern
        return -1;
    }
    return item_ind;
}

static int pathcompLeft(cstr str, char *tmp, const char *expr) {
    int item_ind = -1;
    int tmp_ind = 0;

    for (int i = 0; i < str->len; i++) {
        if (str->string[i] == expr[0]) {
            item_ind = i;
            break;
        }
        tmp[tmp_ind] = str->string[i]; // add to tmp early to cover an L search from split char
        tmp_ind++;
    }

    if (item_ind > -1) {
        if (expr[2] == 'R') {
            tmp_ind = 0;
            for (int i = item_ind + 1; i < str->len; i++) {
                tmp[tmp_ind] = str->string[i];
                tmp_ind++;
            }
        } else if (expr[2] != 'L') { // invalid pattern
            return -1;
        }
        tmp[tmp_ind] = '\0';
    }
    return item_ind;
}

cstr pathcomp(cstr s, const char *expr) {
    cstr str = NULL;
    char tmp[s->len];
    int item_ind;

    if (strlen(expr) < 3) {
        return NULL;
    }
    if (expr[1] == 'L') {
        item_ind = pathcompLeft(s, tmp, expr);
    } else if (expr[1] == 'R') {
        item_ind = pathcompRight(s, tmp, expr);
    } else { // invalid pattern
        return str;
    }
    if (item_ind >= 0) {
        str = cstrInit(tmp);
    }
    return str;
}
