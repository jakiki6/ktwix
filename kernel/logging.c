#include "arch/arch.h"
#include "arch/serial.h"

char *mod_name;

void logging_debug(char *msg) {
	serial_write_string(mod_name);
        serial_write_string(": ");
        serial_write_string("debug: ");
        serial_write_string(msg);
        serial_write_string("\n");
}

void logging_log(char *msg) {
        serial_write_string(mod_name);
        serial_write_string(": ");
        serial_write_string(msg);
        serial_write_string("\n");
}

void logging_panic(char *msg) {
        serial_write_string("\nPANIC: ");
	serial_write_string(mod_name);
        serial_write_string(": ");
        serial_write_string(msg);
        serial_write_string("\n");

	arch_halt();
}

void logging_module(char *name) {
	mod_name = name;
}
