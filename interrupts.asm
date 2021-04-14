extern isr_handler

global isr0
global isr1
global isr2
global isr3
global isr4
global isr5
global isr6
global isr7
global isr8
global isr9
global isr10
global isr11
global isr12
global isr13
global isr14
global isr15
global isr16
global isr17
global isr18
global isr19
global isr20
global isr21
global isr22
global isr23
global isr24
global isr25
global isr26
global isr27
global isr28
global isr29
global isr30
global isr31

global irq0
global irq1

%macro pushaq 0
    push rax
    push rcx
    push rdx
    push rbx
    push rbp
    push rsi
    push rdi
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15
%endmacro

%macro popaq 0
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
    pop rdi
    pop rsi
    pop rbp
    pop rbx
    pop rdx
    pop rcx
    pop rax
%endmacro

isr_default_handler:
	pushaq

	xor rax, rax
	mov rdi, rsp
	cld
	call isr_handler

	popaq
	add rsp, 16

	iretq

isr0:
	push 0
	push 0
	jmp isr_default_handler
isr1:
	push 0
	push 1
	jmp isr_default_handler
isr2:
	push 0
	push 2
	jmp isr_default_handler
isr3:
	push 0
	push 3
	jmp isr_default_handler
isr4:
	push 0
	push 4
	jmp isr_default_handler
isr5:
	push 0
	push 5
	jmp isr_default_handler
isr6:
	push 0
	push 6
	jmp isr_default_handler
isr7:
	push 0
	push 7
	jmp isr_default_handler
isr8:
	push 8
	jmp isr_default_handler
isr9:
	push 0
	push 9
	jmp isr_default_handler
isr10:
	push 10
	jmp isr_default_handler
isr11:
	push 11
	jmp isr_default_handler
isr12:
	push 12
	jmp isr_default_handler
isr13:
	push 13
	jmp isr_default_handler
isr14:
	push 14
	jmp isr_default_handler
isr15:
	push 0
	push 15
	jmp isr_default_handler
isr16:
	push 0
	push 16
	jmp isr_default_handler
isr17:
	push 17
	jmp isr_default_handler
isr18:
	push 0
	push 18
	jmp isr_default_handler
isr19:
	push 0
	push 19
	jmp isr_default_handler
isr20:
	push 0
	push 20
	jmp isr_default_handler
isr21:
	push 21
	jmp isr_default_handler
isr22:
	push 0
	push 22
	jmp isr_default_handler
isr23:
	push 0
	push 23
	jmp isr_default_handler
isr24:
	push 0
	push 24
	jmp isr_default_handler
isr25:
	push 0
	push 25
	jmp isr_default_handler
isr26:
	push 0
	push 26
	jmp isr_default_handler
isr27:
	push 0
	push 27
	jmp isr_default_handler
isr28:
	push 0
	push 28
	jmp isr_default_handler
isr29:
	push 0
	push 29
	jmp isr_default_handler
isr30:
	push 0
	push 30
	jmp isr_default_handler
isr31:
	push 0
	push 31
	jmp isr_default_handler

irq0:
	
irq1: