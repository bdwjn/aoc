GLOBAL _start

SECTION .data

	fn:    db     "input.txt", 0

SECTION .bss

	count: resd   1
	buf:   resb   100000
	
SECTION .text

_start:
	; eax = open("input.txt", O_RDONLY)
	mov eax, 5
	mov ebx, fn
	int 0x80

	cmp eax, -2 ; if (ENOENT) exit(ENOENT);
	jne end_if_not_found
		mov ebx, eax
		mov eax, 1
		int 0x80
	end_if_not_found:
	
	; eax = read(fd, buf, 100000)
	mov ebx, eax ; assumes fd=3=sys_read   :^)
	mov ecx, buf
	mov edx, 100000
	int 0x80
	
	mov esi, buf

line_start:
	call read_num
	mov ebp, ebx     ; ebp = first char pos

	test al, al
	jnz end_if_eof
	if_eof:
		mov eax, 1
		mov ebx, [count]
		int 0x80   ; exit(count)
	end_if_eof:
	
	call read_num
	mov edx, ebx     ; edx = second char pos
	
	lodsb
	mov bl, al       ; bl = test character
	
	mov ecx, ebp
	add ecx, 2
	rep lodsb        ; skip ": " + ecx bytes
	
	xor edi, edi
	
	cmp al, bl
	jne not_first
		mov edi, 1 ; edi = (first char matches) ? 1 : 0
	not_first:
	
	mov ecx, edx
	sub ecx, ebp
	rep lodsb        ; skip (second-first) bytes
	
	cmp al, bl
	jne not_second
		xor edi, 1 ; edi = (first char matches) XOR (second char matches)
	not_second:
	
	add dword [count], edi
	
	skip_to_newline:
	lodsb
	cmp al, 10
	jne skip_to_newline
	
	jmp line_start

read_num:
	xor eax, eax
	xor ebx, ebx

next_digit:
	lodsb
	
	cmp al, '0'      ; if (char<'0' || char>'9') ret
	jl if_not_char
	cmp al, '9'
	jle end_if_not_char
	if_not_char:
		ret
	end_if_not_char:

	xchg eax, ebx    ; ebx *= 10
	mov ecx, 10
	mul ecx        
	xchg eax, ebx
	
	sub al, '0'      ; ebx += digit
	add ebx, eax
	
	jmp next_digit
