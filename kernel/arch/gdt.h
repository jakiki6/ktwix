#include <stdint.h>

#define CS_KERNEL 0x08
#define DS_KERNEL 0x10
#define CS_USER 0x18
#define DS_USER 0x20

typedef struct {
        uint16_t limit;
        uint64_t base;
} __attribute__((packed)) gdt_desc;

void gdt_init();
