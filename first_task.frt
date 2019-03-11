( res bytes -- addr ) 
: save  
	allot ( res bytes -- res addr )
	dup -rot ! 
;
( num -- addr )
: iseven 
	2 % not ( res=1 if num is even res=0 if num is odd )
	1 save   
; 
( num -- )
: parity 
	dup . 
	iseven @ if 
		."  is even number " 
		cr 
	else 
		."  is odd number " 
		cr 
	then 
; 
( a b -- a b a b )
: dup2 
	swap dup rot ( a b -- a a b )
	dup -rot ( a a b -- a b a b )
; 
( num -- addr )
: isprime
	dup 1 <= if 
		drop drop 
		0 1 save 
		exit 
	then 
	dup 2 = if 
		drop drop 
		1 1 save 
		exit 
	then 
	dup 2 % not if 
		drop drop 
		0 1 save 
		exit 
	then 
	3 repeat ( num i )
		dup2 
		% not if 
			drop drop 
			0 1 save 
			exit 
		then 
		1 + dup2 
		dup * < 
	until ( num [++i] )
	1 1 save 
;
( num -- )
: primality 
	dup . 	
	isprime @ if 
		."  is prime number " 
		cr 
	else 
		."  is not prime number " 
		cr 
	then 
; 
( str1 str2 -- [str1+str2] )
: concat 
	0 -rot swap dup2 count swap count + 1 + ( str1 str2 -- 0 str2 str1 [len1+len2+1] ) 
	heap-alloc ( 0 str2 str1 str )
	dup -rot swap 
	repeat 
		dup2 c@ dup -rot 
		if ( if current str hasn't ended ) 
			c! 1 + swap 1 + swap 
		else ( if it's end of str )
			c! 
			drop 1 + rot ( change the str )
			dup if ( if there is another str ) 
				dup2 c@ dup -rot if ( if another str hasn't ended  )
					1 - c! 1 +  
				then  
			then 
		then 
		swap dup -rot 1 - c@ not ( check last concatenated char )
	until ( str 0 str'+1 ) 
	drop drop 
; 
( str1 str2 -- str ) ( str1 str2 will be freed )
: unite-strings 
	dup2 concat 
	dup prints cr 
	swap heap-free 
	swap heap-free
; 
( "Lopyreva" -- string-hash%3 = 0 ) 
( num -- ) 
: collatz 
	dup 1 > if 
		repeat 
			dup 2 % if 
				dup 3 * 1 + 
			else 
				dup 2 / 
			then 
			swap ."  " .  
			dup 1 = 
		until  
	then 
	drop ."  1" cr
; 
