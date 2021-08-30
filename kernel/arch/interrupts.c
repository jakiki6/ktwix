#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "interrupts.h"
#include "arch.h"
#include "gdt.h"

idt_desc desc = { 0 };
idt_entry entries[256] = { 0 };

void register_interrupt(uint64_t handler, uint8_t number, bool trap, bool user) {
	idt_entry *entry = &(entries[number]);
	entry->offset_1 = handler & 0xffff000000000000 >> 48;
	entry->offset_2 = handler & 0x0000ffff00000000 >> 32;
	entry->offset_3 = handler & 0x0000000fffffffff >> 0;
	entry->type_attr = 0b10001110;

	if (user) {
		entry->type_attr = entry->type_attr | 0b01100000;
	}
	if (trap) { 
                entry->type_attr = entry->type_attr | 0b00000001;
        }
}

void interrupts_init() {
	desc.limit = sizeof(entries) - 1;
	desc.base = (uint64_t) &entries - MEM_OFFSET;

	for (int i = 0; i < 256; i++) {
		register_interrupt((uint64_t) &arch_isr_notimplemented, i, false, false);
	}

	log("gdt descriptor at 0x%llx", (uint64_t) &desc);
	arch_load_idt((uint64_t) &desc);

	arch_interrupts_enable();
}
