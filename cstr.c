#include "cstr.h"
#include <ctype.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

cstr cstrInit(const char *from) {
    cstr ret = malloc(sizeof(_cs));
    ret->len = strlen(from);
    ret->alloc = ret->len + 1;
    if (ret->len > 0) {
        ret->string = malloc(sizeof(char) * ret->len + 1);
        memcpy(ret->string, from, ret->alloc);
    } else {
        ret->len = 0;
        ret->alloc = 10; // if initializing with empty string, set to something usable
        ret->string = calloc(ret->alloc, 1);
    }
    return ret;
}

cstr cstrInitSize(const char *from, size_t size) {
    cstr ret = malloc(sizeof(_cs));
    ret->len = strlen(from);
    ret->alloc = size;
    if (ret->len != 0) {
        ret->string = malloc(sizeof(char) * ret->alloc);
        memcpy(ret->string, from, ret->len + 1);
    } else {
        ret->string = calloc(size, 1);
    }
    return ret;
}

// allocates room in s if len > s->alloc
static void makeRoomFor(cstr s, size_t len) {
    if (s->alloc <= len) {
        while (s->alloc <= len) {
            s->alloc *= 2;
        }
        s->string = realloc(s->string, s->alloc);
    }
}

void cstrUpdateString(cstr str, const char *from) {
    size_t from_len = strlen(from);
    if (from_len > str->alloc) {
        makeRoomFor(str, from_len);
    }
    memcpy(str->string, from, from_len + 1);
    str->len = from_len;
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
    size_t from_len = strlen(from);
    size_t needs = from_len + to->len;
    makeRoomFor(to, needs);
    memcpy(to->string + to->len, from, from_len + 1);
    to->len = needs;
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
    *count = 0;
    int elements = 0;
    int slots = 3; // minimum size for 2 elements + NULL
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
    // nothing to split
    if (elements == 0) {
        c[0] = NULL;
        return c;
    }
    if (slots <= elements + 2) { // resize to fit terminating NULL
        c = realloc(c, sizeof(cstr) * (slots + 1));
    }
    if (elements > 0) {
        tmp[ind] = '\0';
        c[elements] = cstrInit(tmp);
        elements++;
        c[elements] = NULL;
        *count = elements;
    }
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
        int new_len = item_ind;
        memcpy(tmp, start, end - start);
        tmp[new_len] = '\0';
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

static int startPathComp(cstr str, char *tmp, const char *expr) {
    int item_ind = -1;
    if (expr[1] == 'L') {
        item_ind = pathcompLeft(str, tmp, expr);
    } else if (expr[1] == 'R') {
        item_ind = pathcompRight(str, tmp, expr);
    } else { // invalid pattern
        return -1;
    }
    return item_ind;
}

// handles multiple patterns in single expr
static int pathloop(cstr str, char *tmp, const char *expr) {
    char new_expr[4];
    int offset = 6;
    int i = 3;
    int item_ind = -1;
    while (i <= strlen(expr) - 1) {
        const char *start = &expr[i];
        const char *end = &expr[offset];
        memcpy(new_expr, start, end - start);
        new_expr[3] = '\0';
        item_ind = startPathComp(str, tmp, new_expr);
        i += 3;
        offset += 3;
        if (item_ind == -1) {
            return -1;
        }
        cstrUpdateString(str, tmp);
    }
    return 0;
}

cstr pathcomp(cstr s, const char *expr) {
    cstr str = NULL;
    char tmp[s->len];
    int item_ind;

    if (strlen(expr) < 3) {
        return NULL;
    }
    item_ind = startPathComp(s, tmp, expr);
    if (item_ind == -1) {
        return NULL;
    }
    str = cstrInit(tmp);
    item_ind = pathloop(str, tmp, expr);
    return str;
}

int intToStr(char *s, int i) {
    char *p = s;
    char swap;
    // convert the int to string (appends to string in reverse)
    while (i > 0) {
        *p++ = '0' + (i % 10);
        i /= 10;
    }
    int len = p - s;
    *p = '\0';
    p--;
    // reverse the string
    while (s < p) {
        swap = *s;
        *s = *p;
        *p = swap;
        s++;
        p--;
    }
    return len;
}

void cstrCatFmt(cstr s, const char *fmt, ...) {
    va_list ap;
    const char *f = fmt;
    long i;
    size_t init_len = s->len;
    size_t fmt_len = strlen(fmt);

    makeRoomFor(s, (init_len + fmt_len) * 2);
    va_start(ap, fmt);
    f = fmt;
    i = init_len;

    while (*f) {
        char next, *str;
        size_t l;
        int num;
        unsigned long long unum;

        switch (*f) {
        case '%':
            next = *(f + 1);
            f++;
            switch (next) {
            case 's':
                str = va_arg(ap, char *);
                l = strlen(str);
                makeRoomFor(s, l);
                memcpy(s->string + i, str, l);
                i += l;
                break;
            case 'd':
                num = va_arg(ap, int);
                char buf[26];
                l = intToStr(buf, num);
                memcpy(s->string + i, buf, l);
                i += l;
                break;
            default:
                s->string[i++] = next;
                break;
            }
            break;
        default:
            s->string[i++] = *f;
            break;
        }
        f++;
    }
    va_end(ap);
    s->string[i] = '\0';
    s->len = i;
}

static bool wholeMatch(const char *str, int start, int end) {
    bool ret = false;
    if (start == 0 || isspace(str[start - 1])) {
        if (isspace(str[end]) || str[end] == '\0') {
            ret = true;
        }
    }
    return ret;
}

void cstrReplace(cstr s, const char *repl, const char *with) {
    size_t repl_len = strlen(repl);
    if (repl_len == 0) {
        return;
    }
    size_t with_len = strlen(with);
    int offset = 0;    // tracks beginning/end of repl in s
    int cur_ind = 0;   // current index in tmp
    const char *start; // position in s to start iteration
    const char *end;   // position in s to end iteration
    int up_to = 0;     // beginning of strstr in s
    char *p = s->string;
    int new_size = s->len * 2;
    char *tmp = NULL;

    while ((p = strstr(p, repl)) != NULL) {
        if (tmp == NULL) {
            tmp = malloc(new_size * sizeof(char));
        }
        start = &s->string[offset];
        end = &p[repl_len];
        up_to = ((end - start) - repl_len) + offset; // 1 position before strstr[0]
        while (offset < up_to) {
            if (cur_ind >= new_size) {
                new_size *= 2;
                tmp = realloc(tmp, new_size);
            }
            tmp[cur_ind] = s->string[offset];
            cur_ind++;
            offset++;
        }
        offset += repl_len;                  // set offset to char immediately after strstr
        for (int i = 0; i < with_len; i++) { // append with to tmp
            if (cur_ind + with_len >= new_size) {
                new_size *= 2;
                tmp = realloc(tmp, new_size);
            }
            tmp[cur_ind] = with[i];
            cur_ind++;
        }
        p += repl_len;
    }
    if (offset > 0) { // append rest of s
        while (offset < s->len) {
            if (cur_ind >= new_size) {
                new_size *= 2;
                tmp = realloc(tmp, new_size);
            }
            tmp[cur_ind] = s->string[offset];
            cur_ind++;
            offset++;
        }
        tmp[cur_ind] = '\0';
        cstrUpdateString(s, tmp);
        free(tmp);
    }
}

cstr cstrReplaceBetween(cstr str, const char *start, const char *end, const char *with, bool whole_match) {
    size_t start_len;
    size_t end_len;
    char *p_start = str->string;
    char *p_end;
    int ind_start, ind_end;

    if ((p_start = strstr(p_start, start)) == NULL) {
        return str;
    }

    start_len = strlen(start);
    // match only whole words and ignore parts of a word
    if (whole_match) {
        while ((p_start = strstr(p_start, start)) != NULL) {
            ind_start = &p_start[0] - str->string;
            ind_end = &p_start[start_len] - str->string;
            if (wholeMatch(str->string, ind_start, ind_end)) {
                break;
            }
            p_start += start_len;
        }
        if (p_start == NULL) {
            return str;
        }
    }

    if ((p_end = strstr(p_start + start_len, end)) == NULL) {
        return str;
    }

    end_len = strlen(end);
    if (whole_match) {
        while ((p_end = strstr(p_end, end)) != NULL) {
            ind_start = &p_end[0] - str->string;
            ind_end = &p_end[end_len] - str->string;
            if (wholeMatch(str->string, ind_start, ind_end)) {
                break;
            }
            p_end += end_len;
        }
        if (p_end == NULL) {
            return str;
        }
    }

    cstr tmp = cstrInitSize("", str->alloc);

    ind_start = (&p_start[0] - str->string) + start_len;
    ind_end = &p_end[0] - str->string + end_len;
    if (!wholeMatch(str->string, ind_end, end_len)) {
        ind_end -= end_len;
    }

    int repl_size = (ind_end - ind_start);
    cstrnCat(tmp, str->string, ind_start);
    cstrCat(tmp, with);
    cstrCat(tmp, str->string + ind_end);
    cstrDealloc(str);
    return tmp;
}

void cstrnCat(cstr str, const char *from, int end) {
    size_t from_len = strlen(from);
    if (end > from_len) {
        return;
    }
    makeRoomFor(str, end + str->len);
    memcpy(str->string + str->len, from, end);
    str->len = str->len + end;
    str->string[str->len] = '\0';
}
