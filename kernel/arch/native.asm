bits 64

%macro isr_entry 0
	push rax
	push rbx
	push rcx
	push rdx
	push rsi
	push rdi
	push rbp
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15

	cld
%endmacro
%macro isr_exit 0
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rbp
	pop rdi
	pop rsi
	pop rdx
	pop rcx
	pop rbx
	pop rax
	iretq
%endmacro

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

global arch_reboot
arch_reboot:
	lidt [fake_idt_desc]
	int 0x69

	mov dx, 0x0cf9
	mov al, 0x0e
	out dx, al

	jmp 0xffff0

global spinlock_lock
spinlock_lock:
	lock bts qword [rdi], 0
	jc .spin
	ret
.spin:	pause
	test qword [rdi], 1
	jnz .spin
	jmp spinlock_lock

global spinlock_release
spinlock_release:
	lock btr qword [rdi], 0
	ret

global arch_isr_notimplemented
extern log
arch_isr_notimplemented:
	isr_entry

	mov rdi, msg
	call log

	isr_exit

section .data
fake_idt_desc:
	db 0
	dq 0
msg:	db "E"
