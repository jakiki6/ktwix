#pragma once

#include <stddef.h>
#include <stdint.h>

#define ALIGN(x) __attribute__((aligned(x)))
#define PACKED __attribute__((packed))

#define __memory_offset 0xffffffff80000000

typedef struct {
        size_t pages;
} heap_data;

void *kmalloc(size_t size);
void *kcalloc(size_t size);
void kfree(void *address);

void panic(const char* format, ...);
void log(const char* format, ...);
void debug(const char* format, ...);
void module(char *name);
uint64_t UD(uint64_t num, uint64_t div);
