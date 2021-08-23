#pragma once

#include <stdint.h>
#include <klib/klib.h>

#define CS_KERNEL 0x08
#define DS_KERNEL 0x10
#define CS_USER CS_KERNEL + 0x10
#define DS_USER DS_KERNEL + 0x10

#define GDT_SEGMENT (0b00010000)
#define GDT_PRESENT (0b10000000)
#define GDT_USER (0b01100000)
#define GDT_EXECUTABLE (0b00001000)
#define GDT_READWRITE (0b00000010)
#define GDT_LONG_MODE_GRANULARITY 0b0010
#define GDT_FLAGS 0b1100
#define GDT_USER (0b01100000)

typedef struct {
        uint16_t size;
        uint64_t base;
} PACKED ALIGN(8) gdt_desc;

typedef struct {
	uint16_t limit0_15;
	uint16_t base0_15;
	uint8_t base16_23;
	uint8_t flags;
	uint8_t limit16_19 : 4;
	uint8_t granularity : 4;
	uint8_t base24_31;
} PACKED gdt_entry;

void gdt_init();
