#include <stdint.h>

#include "arch/gdt.h"
#include "arch/arch.h"

extern int __memory_offset;

static gdt_desc desc = { 0 };
static uint64_t entries[5] = { 0 };

void gdt_init() {
	entries[1] = 0x00209A0000000000;		// code for kernel
	entries[2] = 0x0000920000000000;		// data for kernel
        entries[3] = entries[1] | 0x600000000000;	// code for userspace
        entries[4] = entries[2] | 0x600000000000;	// data for userspace

	desc.limit = (3 * 8 - 1);
	desc.base = (uint64_t) &entries - __memory_offset;

	arch_lgdt((uint64_t) &desc - __memory_offset);
	arch_set_code_segment(CS_KERNEL);
	arch_set_data_segments(DS_KERNEL);
}
