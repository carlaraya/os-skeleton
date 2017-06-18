// main.c -- Defines the C-code kernel entry point, calls initialisation routines.
// Made for JamesM's tutorials 
#include "monitor.h"
#include "descriptor_tables.h"
#include "timer.h"
#include "paging.h"
struct multiboot;

int main(struct multiboot *mboot_ptr) {
    init_descriptor_tables();
    asm volatile("sti"); // enable hardware interrupts
    monitor_clear();
    monitor_write("Hello, world!\n");

    initialise_paging();
    monitor_write("Hello, paging world!\n");

    u32int *ptr = (u32int*)0xA0000000;
    u32int do_page_fault = *ptr;

    return 0;
    /*asm volatile("int $0x0 ");
    init_timer(50);
    */

    /*
    int i;
    for (i = 0; i < 10000; i++) {
        monitor_write_dec(i);
        monitor_write("\n");
        if (i == 6969) {
            PANIC("FUCK FUCK I'M PANICKINGGGGG");
        }
    }
    */
    return 0;
}
