#include <stdint.h>
#include <stddef.h>
#include <klib/klib.h>
#include <klib/memutils.h>

#include "arch/arch.h"
#include "arch/paging.h"
#include "boot/boot.h"
#include "arch/interrupts.h"
#include "arch/gdt.h"
#include "arch/serial.h"
#include "kmalloc.h"

extern uint64_t __text_start, __text_end;

void kmain(boot_info *handover) {
	// disable irq because we don't have idt yet
	arch_interrupts_disable();

	module("boot");

	log("ktwix 0.01");
	log("memory offset: 0x%llx", __memory_offset);

	// setup serial in case of a panic
	serial_init();

	// log some data
	log("usable memory: %lluMB of %lluMB", handover->memory_usable / 1024 / 1024, handover->total_memory / 1024 / 1024);

	// init gdt
	log("loading gdt");
	gdt_init();

	// init interrupts
	log("loading idt");
	interrupts_init();

	// init kmalloc
	log("init kmalloc");
	kmalloc_init(handover->memory_map->memmap, handover->memory_entries);

	// setup paging
	log("init paging");
	paging_init(handover);

	log("boot completed");
	module("kernel");

	for (;;);
}
