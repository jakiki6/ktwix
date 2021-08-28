#include <stdint.h>
#include <stddef.h>
#include <klib/klib.h>
#include <klib/memutils.h>

#include "kmalloc.h"

static uint8_t *kmalloc_map;
size_t last_used_index = 0;
uint64_t highest_page = 0;

void kmalloc_init(struct stivale2_mmap_entry *memory_map, size_t memory_entries) {
	for (size_t i_entry = 0; i_entry < memory_entries; i_entry++) {
		struct stivale2_mmap_entry entry = memory_map[i_entry];

		if (entry.type != STIVALE2_MMAP_USABLE)
			continue;

		memset((void *) entry.base, 0, entry.length);

		uint64_t top = entry.base + entry.length;

		if (top > highest_page)
			highest_page = top;
	}

	size_t memory_size = highest_page + (PAGE_SIZE - 1) / PAGE_SIZE;
	size_t bitmap_size = memory_size / 8;

	log("kmalloc bitmap size: 0x%llx", bitmap_size);

	for (size_t i = 0; i < memory_entries; i++) {
		struct stivale2_mmap_entry entry = memory_map[i];

		if (entry.type != STIVALE2_MMAP_USABLE)
			continue;


		if (entry.length >= bitmap_size) {
			kmalloc_map = (uint8_t *) entry.base + MEM_OFFSET;
			for (uint8_t *ptr = kmalloc_map; (size_t) ptr < bitmap_size; ptr++) {
				*ptr = 0xff;
			}
			entry.base += bitmap_size;
			entry.length -= bitmap_size;

			break;
		}
	}

	log("kmalloc usable memory:");

	for (size_t j = 0; j < memory_entries; j++) {
		struct stivale2_mmap_entry entry = memory_map[j];
		if (entry.type != STIVALE2_MMAP_USABLE)
			continue;

		for (size_t length = 0; length < entry.length; length += PAGE_SIZE)
			CLEARBIT((entry.base + length) / PAGE_SIZE);

		log("\tbase: 0x%llx length 0x%llx", entry.base + MEM_OFFSET, entry.length);
	}

	log("kmalloc init:\n\tkmalloc map: 0x%llx\n\thighest page: 0x%llx", (uint64_t) kmalloc_map, highest_page);
}

void *kmalloc_allocate_page() {
	return kmalloc_allocate_pages(1);
}

void *kmalloc_allocate_pages(size_t count) {
	size_t index = last_used_index;
	size_t runs = 0;
	size_t hits = 0;
	size_t page = index;
	while (runs < (highest_page - MEM_OFFSET) / PAGE_SIZE) {
		if (GETBIT(index) == false) {
			if (page == (size_t) -1) {
				page = index;
			}

			if (hits++ == count) {
				for (size_t i = 0; i < count; i++) {
					SETBIT(i + page);
				}
				debug("allocated %u page(s) at address 0x%llx", count, page * PAGE_SIZE + MEM_OFFSET);
				last_used_index = index;
				return (void *) (page * PAGE_SIZE + MEM_OFFSET);
			}
		} else {
			hits = 0;
			page = -1;
		}
		index++;
		index = index % ((highest_page - MEM_OFFSET) / PAGE_SIZE);
		runs++;
	}

	module("kmalloc");
	panic("Out of memory (ranboo moment)");
	for (;;);
}

void *kmalloc_callocate_page() {
	return kmalloc_callocate_pages(1);
}

void *kmalloc_callocate_pages(size_t count) {
	uint8_t *address = (uint8_t *) kmalloc_allocate_pages(count);

	if (address == NULL) {
	        return NULL;
	}

	memset(address, 0, count * PAGE_SIZE);

	return address;
}

void kmalloc_free_page(void *address) {
	kmalloc_free_pages(address, 1);
}

void kmalloc_free_pages(void *address, size_t count) {
	uint64_t start_page = (uint64_t) (address - MEM_OFFSET) / PAGE_SIZE;

	for (size_t i = 0; i < count; i++) {
		CLEARBIT(i + start_page);
	}

	debug("freed %u page(s) at 0x%llu", count, address);
}

uint8_t *kmalloc_get_map() {
	return kmalloc_map;
}
