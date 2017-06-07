// pls dont judge ser huan chou

#ifndef VERONICA_H
#define VERONICA_H

// Some nice typedefs, to standardise sizes across platforms.
// These typedefs are written for 32-bit X86.
typedef unsigned int   u32int;
typedef          int   s32int;
typedef unsigned short u16int;
typedef          short s16int;
typedef unsigned char  u8int;
typedef          char  s8int;

void memcpy(u8int *dest, const u8int *src, u32int len);
void memset(u8int *dest, u8int val, u32int len);

unsigned int strlen(const char *s);
int strcmp(const char *s1, const char *s2);

#endif
