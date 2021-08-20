#include <stdint.h>
#include <klib/klib.h>

#include "arch/gdt.h"
#include "arch/arch.h"

static ALIGN(8) gdt_desc desc = { 0 };
static ALIGN(8) gdt_entry entries[5] = { 0 };

gdt_entry create_gdt_entry(uint32_t base, uint32_t limit, uint8_t granularity, uint8_t flags) {
	return (gdt_entry) {
		.limit0_15 = (uint16_t) ((limit) & 0xFFFF),
		.base0_15 = (uint16_t) ((base) & 0xFFFF),
		.base16_23 = (uint8_t) (((base) >> 16) & 0xFF),
		.flags = (flags),
		.limit16_19 = ((limit) >> 16) & 0x0F,
		.granularity = (granularity),
		.base24_31 = (uint8_t) (((base) >> 24) & 0xFF),
	};
}

#define bake_entry(flags, granularity) (create_gdt_entry(0, 0, granularity, flags))

void gdt_init() {
	entries[0] = create_gdt_entry(0, 0, 0, 0);
	entries[1] = bake_entry(GDT_PRESENT | GDT_SEGMENT | GDT_READWRITE | GDT_EXECUTABLE, GDT_LONG_MODE_GRANULARITY);
	entries[2] = bake_entry(GDT_PRESENT | GDT_SEGMENT | GDT_READWRITE, 0);
	entries[3] = bake_entry(GDT_PRESENT | GDT_SEGMENT | GDT_READWRITE | GDT_EXECUTABLE | GDT_USER, GDT_LONG_MODE_GRANULARITY);
        entries[4] = bake_entry(GDT_PRESENT | GDT_SEGMENT | GDT_READWRITE | GDT_USER, 0);

	desc.size = sizeof(entries) - 1;
	desc.base = (uint64_t) &entries;

	uint64_t gdt_addr = ((uint64_t) &desc) - __memory_offset;
	log("gdt descriptor at 0x%llx", gdt_addr);
	arch_load_gdt(gdt_addr);
}
