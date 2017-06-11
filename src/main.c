// main.c -- Defines the C-code kernel entry point, calls initialisation routines.
// Made for JamesM's tutorials 
#include "monitor.h"
#include "descriptor_tables.h"
#include "timer.h"
struct multiboot;

int main(struct multiboot *mboot_ptr) {
    init_descriptor_tables();
    asm volatile("sti"); // enable hardware interrupts

    int x;
    monitor_clear();
    monitor_write("Ang dali ng MP!\nlol\n");
    monitor_write_hex(0xDEADBEEF);
    monitor_write("\n");
    monitor_write_dec_s(-12);
    monitor_write("\n");
    monitor_write("\n");
    asm volatile("int $0x0 ");
    monitor_write("sldfj\n");
    init_timer(50);
    return 0;
}
