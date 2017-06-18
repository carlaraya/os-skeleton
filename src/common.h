// common.h -- Defines some global functions.
// From JamesM's kernel development tutorials.

#include "veronica.h"

#ifndef COMMON_H
#define COMMON_H

void outb(u16int port, u8int value);
u8int inb(u16int port);
u16int inw(u16int port);

#define PANIC(msg) panic(msg, __FILE__, __LINE__); 

extern void panic(const char *message, const char *file, u32int line);

#endif
