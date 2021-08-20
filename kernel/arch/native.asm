bits 64
section .text

global arch_load_gdt
arch_load_gdt:
	lgdt [rdi]
	mov rax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	push 0x08
	push .trampoline
	retq
.trampoline:
	ret

global arch_load_idt
arch_load_idt:
	lidt [rdi]
	ret
