%define _lw 0 						;last word

%macro native 3

	section .data
		wh_ %+ %2 : dq _lw 			;previous word
			db 0
			db %1, 0			;name
			db %3				;immediate flag
	%define _lw wh_ %+ %2
		xt_ %+ %2 : dq i_ %+ %2	;execution token

	section .text
		i_ %+ %2 :				;implementatio
%endmacro


%macro native 2
	native %1, %2, 0
%endmacro


%macro colon 3

	section .data
		wh_ %+ %2 : dq _lw			;previous word
			db 0
			db %1, 0			;name
			db %3				;immediate flag
	%define _lw wh_ %+ %2
		xt_ %+ %2 : dq i_docol			;docol implementation	
%endmacro


%macro colon 2
	colon %1, %2, 0
%endmacro
