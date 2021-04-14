include ../flags/kernel.mk

TARGET := kernel.elf
SYMBOLS := kernel.sym

LIBS := ../libraries/klib.a ../libraries/libprintf.a

ROOT=$(shell pwd)

LDINTERNALFLAGS := $(LDINTERNALFLAGS) \
	-Tlinker.ld

INTERNALCFLAGS := $(INTERNALCFLAGS) \
	-I $(ROOT)/include/		\
	-I $(ROOT)				\
	-I $(ROOT)/../thirdparty/stivale/ \
	-I $(ROOT)/../libraries/

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJ)
	@echo Linking kernel
	@$(LD) $(LDINTERNALFLAGS) -o $@ $(OBJ) $(LIBS)
	@$(OBJCOPY) --only-keep-debug $@ $(SYMBOLS)
#	$(STRIP) $@

%.o: %.c
	@echo Building kernel object $@
	@$(CC) $(CFLAGS) $(INTERNALCFLAGS) -c $< -o $@

%.o: %.asm
	@$(ASM) -f elf64 $< -o $@

clean:
	@echo Cleaning kernel
	@rm -rf $(TARGET) $(OBJ) $(SYMBOLS)
