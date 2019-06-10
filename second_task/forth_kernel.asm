%include "macro.inc"
%include "basic_words.inc"
%include "initial_process.inc"

global _start
%define pc r15
%define w r14
%define rstack r13

section .bss
resq 1023
rstack_start: resq 1
stack_start: resq 1
in_fd: resq 1
input_buffer: resb 1024		; buffer to read textual words into
user_dictionary:  resq 65536	; data for words
user_mem: resq 65536		; global data for user
state: resq 1			; changes to 1 if compiling, 0 by default

section .data
dict_pointer: dq user_dictionary

section .rodata
error_message: db ": no such word", 10, 0

section .text
_start:	mov rstack, rstack_start
	mov pc, program
	jmp next

next:	mov w, [pc]
	add pc, 8
	jmp [w]

program: dq xt_start
