// main.c -- Defines the C-code kernel entry point, calls initialisation routines.
// Made for JamesM's tutorials 
#include "monitor.h"
#include "descriptor_tables.h"
struct multiboot;

int main(struct multiboot *mboot_ptr) {
    init_descriptor_tables();

    int x;
    monitor_clear();
    monitor_write("Ang dali ng MP!\nlol\n");
    monitor_write_hex(0xDEADBEEF);
    monitor_write("\n");
    monitor_write_dec_s(-12);
    monitor_write("\n");
    monitor_write("\n");
    asm volatile("int $0x0 ");
    asm volatile("int $0x1 ");
    asm volatile("int $0x2 ");
    asm volatile("int $0x3 ");
    asm volatile("int $0x4 ");
    asm volatile("int $0x5 "); 
    asm volatile("int $0x6 ");
    asm volatile("int $0x7 ");
    asm volatile("int $0x8 ");/*
    asm volatile("int $0x9 ");
    asm volatile("int $0xA ");
    asm volatile("int $0xB ");
    asm volatile("int $0xC ");
    asm volatile("int $0xD ");
    asm volatile("int $0xE ");
    asm volatile("int $0xF ");
    asm volatile("int $0x10");
    asm volatile("int $0x11");
    asm volatile("int $0x12");
    asm volatile("int $0x13");
    asm volatile("int $0x14");
    asm volatile("int $0x15");
    asm volatile("int $0x16");
    asm volatile("int $0x17");
    asm volatile("int $0x18");
    asm volatile("int $0x19");
    asm volatile("int $0x1A");
    asm volatile("int $0x1B");
    asm volatile("int $0x1C");
    asm volatile("int $0x1D");
    asm volatile("int $0x1E");
    asm volatile("int $0x1F"); */
    return 0;
}
