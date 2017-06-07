// pls dont judge ser huan chou

#include "veronica.h"

// Copy len bytes from src to dest.
void memcpy(u8int *dest, const u8int *src, u32int len)
{
    const u8int *sp = (const u8int *)src;
    u8int *dp = (u8int *)dest;
    for(; len != 0; len--) *dp++ = *sp++;
}

// Write len copies of val into dest.
void memset(u8int *dest, u8int val, u32int len)
{
    u8int *temp = (u8int *)dest;
    for ( ; len != 0; len--) *temp++ = val;
}

unsigned int strlen(const char *s) {
    unsigned int i;
    for (i = 0; s[i]; i++);
    return i;
}

int strcmp(const char *s1, const char *s2) {
    int i;
    for (i = 0; s1[i] == s2[i] && s1[i] && s2[i]; i++);
    return s1[i] - s2[i];
}
