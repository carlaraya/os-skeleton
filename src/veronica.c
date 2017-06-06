// pls dont judge ser huan chou

#include "veronica.h"

void *memcpy(void *dest, const void *src, size_t n) {
    char *src_ch = (char *) src;
    char *dest_ch = (char *) dest;
    for (size_t i = 0; i < n; i++) dest_ch[i] = src_ch[i];
    return dest;
}

void *memset(void *s, int c, size_t n) {
    char *s_ch = (char *) s, *c_ch = (char *) &c;
    for (size_t i = 0; i < n; i++) s_ch[i] = c_ch[i];
    return s;
}

size_t strlen(const char *s) {
    size_t i;
    for (i = 0; s[i]; i++);
    return i;
}

int strcmp(const char *s1, const char *s2) {
    int i;
    for (i = 0; s1[i] == s2[i] && s1[i] && s2[i]; i++);
    return s1[i] - s2[i];
}
