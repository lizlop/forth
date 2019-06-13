; ( -- input_buffer_address )
const 'input_buf', input_buf

; ( -- last_word_address )
const 'last_word', last_word

; ( -- state_address )
const 'state', state

; ( -- dict_pointer_address )
const 'dict_pointer', dict_pointer

;( buffer -- char ) 
; read char in the buffer and copy to the stack
colon 'read-char', read_char
	dq xt_ret_push
	dq xt_lit, -1	
	dq xt_forth_sp
	dq xt_ret_push	
	dq xt_lit, 0
	dq xt_ret_pop
	dq xt_lit, 1
	TIMES 3 db xt_lit, 0
	dq xt_syscall
	dq xt_drop, xt_drop, xt_dup
	dq xt_ret_pop, xt_write_char
	dq xt_exit

;( buffer -- 0/start_address )
; read word in the buffer
colon 'read-word', read_word
	dq xt_ret_push	;address of first char
.skip:	
	dq xt_ret_fetch, xt_read_char
	dq xt_ret_push
	dq xt_ret_fetch
	branch0 .no_input
	dq xt_ret_fetch, xt_lit, ' ', xt_equals
	dq xt_ret_fetch, xt_lit, 10, xt_equals, xt_or
	dq xt_ret_fetch, xt_lit, 13, xt_equals, xt_or,
	dq xt_ret_pop, xt_lit, 9, xt_equals, xt_or, xt_not
	branch0 .skip
	dq xt_lit, 1, xt_ret_push	;string length
.read:
	dq xt_ret_fetch, xt_ret_fetch, xt_plus	
	dq xt_read_char
	dq xt_ret_push
	dq xt_ret_fetch
	branch0 .quit
	dq xt_ret_fetch, xt_lit, ' ', xt_equals
	dq xt_ret_fetch, xt_lit, 10, xt_equals, xt_or
	dq xt_ret_fetch, xt_lit, 13, xt_equals, xt_or,
	dq xt_ret_pop, xt_lit, 9, xt_equals, xt_or, xt_not
	branch0 .quit
	dq xt_ret_fetch, xt_lit, 254, xt_equals, xt_not
	branch0 .quit
	dq xt_ret_pop, xt_lit, 1, xt_plus, xt_ret_push
	branch .read
.quit:
	dq xt_lit, 0	
	dq xt_ret_pop, xt_ret_fetch, xt_plus, xt_write
	dq xt_ret_pop
    	dq xt_exit
.no_input:
	dq xt_ret_pop, xt_ret_pop, xt_drop
	dq xt_exit

; ( a b -- a b a b )
colon 'dup2', double_dup
	dq xt_swap, xt_dup		; ( a b -- b a a )
	dq xt_ret_push, xt_swap		; ( b a a -- a b )
	dq xt_dup, xt_ret_pop		; ( a b -- a b b a )
	dq xt_swap 			; ( a b -- a b a b )
	dq xt_exit

;( str1 str2 -- 0/1 )
colon 'string-equals', string_equals
.start:
	dq xt_double_dup, xt_fetch_char, xt_swap, xt_fetch_char, xt_equals
	branch0 .no
	dq xt_dup, xt_fetch_char
	branch0 .yes
	dq xt_lit, 1, xt_plus, xt_swap, xt_lit, 1, xt_plus, xt_swap
	branch .start
.no:
	dq xt_drop, xt_drop, xt_lit, 0, xt_exit
.yes:
	dq xt_drop, xt_drop, xt_lit, 1, xt_exit 

;( buffer -- 0/(word 1) )
colon 'find', find
	dq xt_ret_push
	dq xt_last_word
	dq xt_state
	branch0 .loop		
	dq xt_fetch
.loop:
	dq xt_dup
	branch0 .not_found	
	dq xt_dup, xt_lit, 9, xt_plus, xt_ret_fetch
	dq xt_string_equals, xt_not
	branch0 .found
	dq xt_fetch
	branch .loop
.not_found:
	dq xt_ret_pop, xt_drop, xt_drop, xt_lit, 0, xt_exit
.found:
	dq xt_ret_pop, xt_drop, xt_lit, 1, xt_exit

; ( word_addr -- xt )
colon "get-xt", get_xt
	dq xt_lit, 9, xt_plus    
.loop:
	dq xt_dup, xt_fetch_char
	branch0 .end
	dq xt_lit, 1, xt_plus
	branch .loop
.end:
	dq xt_lit, 2, xt_plus, xt_exit

; ( xt -- )
; add xt to the current word in dictionary
colon ',', comma
	dq xt_dict_pointer, xt_fetch, xt_write
	dq xt_lit, 8, xt_dict_pointer, xt_fetch, xt_plus
	dq xt_dict_pointer, xt_write, xt_exit

colon '>', more
	dq xt_double_dup, xt_less, xt_ret_push, xt_equals, xt_ret_pop, xt_or, xt_not
	dq xt_exit

; ( buffer -- num 0/1 )
colon 'parse-uint', parse_uint
	dq xt_ret_push, xt_lit, 0, xt_ret_push, xt_lit, 0, xt_lit, 1
.loop:
	dq xt_ret_fetch, xt_ret_fetch, xt_plus, xt_fetch_char
	dq xt_dup
	branch0 .success
	dq xt_lit, '0', xt_minus
	dq xt_dup, xt_lit, 0, xt_less, xt_not
	branch0 .error
	dq xt_dup, xt_lit, 9, xt_more, xt_not
	branch0 .error
	dq xt_mul, xt_plus, xt_lit, 10
	dq xt_ret_pop, xt_lit, 1, xt_plus, xt_ret_push
	branch .loop
.error:
	dq xt_drop, xt_drop, xt_ret_pop, xt_ret_pop, xt_drop, xt_drop
	dq xt_lit, 0, xt_exit
.success:
	dq xt_drop, xt_drop, xt_ret_pop, xt_ret_pop, xt_drop, xt_drop
	dq xt_lit, 1, xt_exit

; ( buffer -- num 0/1 )
colon 'parse-int', parse_int
	dq xt_dup, xt_fetch_char, xt_lit, '-', xt_equals, xt_not
	branch0 .signed
	dq xt_parse_uint, xt_exit
.signed:
	dq xt_dup, xt_lit, 1, xt_plus
	dq xt_parse_uint, xt_swap
	dq xt_lit, 0, xt_swap, xt_minus, xt_swap, xt_exit

; ( buffer -- length )
colon 'str-length', str_length
	dq xt_ret_push, xt_lit, 0, xt_ret_push
.loop:
	dq xt_ret_fetch, xt_ret_fetch, xt_plus, xt_fetch_char
	branch0 .end
	dq xt_ret_pop, xt_lit, 1, xt_plus, xt_ret_push
	dq xt_ret_fetch, xt_lit, 254, xt_equals, xt_not
	branch0 .end
	branch .loop
.end:
	dq xt_ret_pop, xt_ret_pop, xt_drop, xt_exit

; ( buffer -- )
colon 'print-str', print_str
	dq xt_dup, xt_str_length, xt_ret_push, xt_ret_push
	dq xt_lit, 1
	dq xt_lit, 0
	dq xt_lit, xt_ret_pop
	dq xt_lit, xt_ret_pop
	TIMES 3 dq xt_lit, 0
	dq xt_syscall, xt_drop, xt_drop, xt_exit

colon "interpret", interpret
.loop:
	dq xt_input_buf, xt_read_word 	; ( -- 0/buffer )
	dq xt_zero_branch, .end_of_input
	dq xt_input_buf	
.start:
	dq xt_dup, xt_find		; ( inbuf -- inbuf dictaddr )
	dq xt_dup
	branch0 .number
	dq xt_get_xt			; ( inbuf dictaddr -- inbuf xt )
	dq xt_state, xt_fetch
	branch0 .interpreting
	dq xt_dup
	dq xt_lit, 1, xt_minus, xt_fetch_char, xt_not
	branch0 .interpreting
	dq xt_swap, xt_drop		; ( inbuf xt -- xt)
	dq xt_comma
	dq xt_exit
.interpreting: 
	dq xt_swap, xt_drop		; ( inbuf xt -- xt )
	dq xt_execute
	dq xt_exit
; ( inbuf dictaddr )
.number: 
	dq xt_drop, xt_dup
	dq xt_parse_int ; ( inbuf num 0/1 )
	branch0 .no_word
	; ( inbuf num )
	dq xt_swap, xt_drop
	dq xt_state, xt_fetch  ; if interpreting, that's it
	branch0 .exit
	; when compiling, check if it was after a branch
	; ( num )
	dq xt_dict_pointer, xt_lit, 8, xt_minus, xt_fetch
	; ( num prev_xt )
	dq xt_dup
	dq xt_lit, xt_zero_branch		; ( num prev_xt prev_xt xt_zero_branch )
	dq xt_equals, xt_not
	branch0 .is_branch
	dq xt_dup
	dq xt_lit, xt_branch		; ( num prev_xt prev_xt xt_branch )
	dq xt_equals, xt_not
	branch0 .is_branch
	dq xt_lit, xt_lit, xt_comma	
; ( num prev_xt )
.is_branch:
	dq xt_drop
	dq xt_comma
	dq xt_exit
; ( inbuf num )
.no_word: 
	dq xt_drop
	dq xt_print_str
	dq xt_lit, error_message, xt_print_str
.exit:
	branch .loop
.end_of_input:
	dq xt_exit

colon 'bye', bye
	dq xt_lit, 60
	dq xt_lit, 0
	dq xt_lit, 0
	dq xt_lit, 0
	dq xt_lit, 0
	dq xt_lit, 0
	dq xt_lit, 0
	dq xt_syscall

colon "c,", char_comma
	  dq xt_dict_pointer, xt_fetch, xt_write_char
	  dq xt_dict_pointer, xt_fetch, xt_lit, 1, xt_plus
	  dq xt_dict_pointer, xt_fetch, xt_exit

; ( addr str - ) copy str to addr
colon "string-copy", string_copy
.loop:
	dq xt_double_dup, xt_fetch_char, xt_ret_push, xt_ret_fetch, xt_swap, xt_write_char
	dq xt_lit, 1, xt_plus, xt_swap, xt_lit, 1, xt_plus, xt_swap
	dq xt_ret_pop, xt_not
	branch0 .loop
	dq xt_drop, xt_drop, xt_exit

colon 'create', create
	dq xt_dict_pointer, xt_fetch                		; word start
	dq xt_last_word, xt_fetch, xt_comma 			; fill `link`
	dq xt_last_word, xt_write           			; update `last_word`
	dq xt_lit, 0, xt_char_comma
	dq xt_dup, xt_dict_pointer, xt_fetch, xt_swap, xt_string_copy
	dq xt_str_length, xt_dict_pointer
	dq xt_fetch, xt_plus, xt_lit, 1, xt_plus, xt_dict_pointer, xt_write
	dq xt_char_comma
	dq xt_exit

colon ':', colon
.restart:
	dq xt_input_buf, xt_read_word
	branch0 .restart
	dq xt_lit, 0, xt_input_buf, xt_create
	dq xt_lit, 1, xt_state, xt_write
	dq xt_lit, i_docol, xt_comma
	dq xt_exit
