#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "interrupts.h"
#include "arch.h"
#include "gdt.h"

idt_desc desc = { 0 };
idt_entry entries[256] = { 0 };

void register_interrupt(uint64_t handler, uint8_t number, bool user) {
	idt_entry *entry = &(entries[number]);
	entry->offset_1 = handler & 0xffff000000000000 >> 48;
	entry->offset_2 = handler & 0x0000ffff00000000 >> 32;
	entry->offset_3 = handler & 0x0000000fffffffff >> 0;

	if (user) {
		entry->selector = CS_USER;
	} else {
		entry->selector = CS_KERNEL;
	}
}

void interrupts_init() {
	desc.limit = sizeof(entries) - 1;
	desc.base = (uint64_t) &entries;

	arch_load_idt((uint64_t) &desc);
}
