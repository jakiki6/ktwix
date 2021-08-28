export PATH:=$(shell toolchain/use-it.sh):$(PATH)

all: run

run: os.img
	@echo Booting system
	@rm log.txt 2> /dev/null || true
	@qemu-system-x86_64 -s -m 256M -drive format=raw,file=$< -D log.txt -d int -serial stdio -machine smm=off -no-reboot

kernel_all:
	@echo Making kernel
	@make --no-print-directory -C kernel all
lib_all:
	@echo Making libs
	@make --no-print-directory -C libraries all
kernel_clean:
	@make --no-print-directory -C kernel clean
lib_clean:
	@make --no-print-directory -C libraries clean

os.img: lib_all kernel_all
	@echo Making system image
	@qemu-img create $@ 64M > /dev/null 2> /dev/null

	@parted -s $@ mklabel gpt
	@parted -s $@ mkpart primary 2048s 100%

	@echfs-utils -g -p0 $@ quick-format 512 > /dev/null

	@echfs-utils -g -p0 $@ import config/limine.cfg limine.cfg
	@echfs-utils -g -p0 $@ import kernel/kernel.elf kernel.elf

	@limine-install $@ > /dev/null 2> /dev/null

clean: kernel_clean lib_clean
	@rm os.img 2> /dev/null || true

.PHONY: all run clean
