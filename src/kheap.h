#include "common.h"

u32int kmalloc(u32int sz); // vanilla (normal).
u32int kmalloc_a(u32int sz);  // page aligned.
u32int kmalloc_p(u32int sz, u32int *phys); // returns a physical address.
u32int kmalloc_ap(u32int sz, u32int *phys); // page aligned and returns a physical address.

u32int kmalloc_int(u32int sz, int align, u32int *phys);
