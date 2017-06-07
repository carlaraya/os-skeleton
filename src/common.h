// common.h -- Defines typedefs and some global functions.
// From JamesM's kernel development tutorials.

#include "veronica.h"

#ifndef COMMON_H
#define COMMON_H

void outb(u16int port, u8int value);
u8int inb(u16int port);
u16int inw(u16int port);

#endif
