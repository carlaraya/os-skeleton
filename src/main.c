// main.c -- Defines the C-code kernel entry point, calls initialisation routines.
// Made for JamesM's tutorials 
#include "monitor.h"
struct multiboot;

int main(struct multiboot *mboot_ptr) {
  // All our initialisation calls will go in here.
  int x;
  monitor_clear();
  monitor_write("Ang dali ng MP!\nlol\n");
  monitor_write_hex(0xDEADBEEF);
  monitor_write("\n");
  monitor_write_dec_s(-12);
  monitor_write("\n");
  return 0;
}
