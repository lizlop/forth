global _start
%include "macro.inc"
%define pc r15
%define w r14
%define rstack r13

section .text
%include "basic_words.inc"
%include "initial_process.inc"

section .bss
resq 1023
rstack_start: resq 1
stack_start: resq 1
in_fd: resq 1
input_buf: resb 1024		; buffer to read textual words into
user_dictionary:  resq 65536	; data for words
user_mem: resq 65536		; global data for user
state: resq 1			; changes to 1 if compiling, 0 by default

section .data
dict_pointer: dq user_dictionary
mem_pointer: dq user_mem
last_word: dq _lw

section .rodata
error_message: db ": no such word", 10, 0

section .text
_start:	mov rstack, rstack_start
	mov pc, program
	jmp next

next:	mov w, [pc]
	add pc, 8
	jmp [w]

program:
	dq xt_lit, 0
	dq xt_interpret
	dq xt_bye
