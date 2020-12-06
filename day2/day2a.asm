global _start

section .data
	fn: db "input.txt", 0

section .bss

	sum: resd 1
	buf: resb 100000

section .text

_start:
	mov eax, 5
	mov ebx, fn
	mov ecx, 2
	int 0x80 ; eax = open("input.txt", O_RDONLY)
	
	test eax, eax
	jnz end_if_file_not_found ; if (!fd)
		mov eax, 1
		mov ebx, 1
		int 0x80 ; exit(1)
	end_if_file_not_found:
	
	mov ebx, eax
	mov ecx, buf
	mov edx, 100000
	int 0x80 ; eax = read(fd, buf, 100000)
	
	mov esi, buf

line_start:
	call read_num
	mov edi, ebx  ; edi = min count
	
	test al, al
	jnz end_if_eof
		mov eax, 1
		mov ebx, [sum]
		int 0x80 ; exit(sum)
	end_if_eof:
	
	mov ecx, 10   ; (used in scan_pass and read_num)

	call read_num
	mov edx, ebx  ; edx = max count
	
	lodsb
	mov bl, al    ; bl = test character

	lodsw         ; skip colon/space
	
	xor ebp, ebp  ; ebp = char counter
	
scan_pass:
	lodsb
	cmp al, cl    ; cl = 10 = '\n'
	je check_pass
	cmp al, bl
		jne scan_pass
		inc ebp
		jmp scan_pass

check_pass:
	cmp ebp, edi
	jl wrong_pass
	cmp ebp, edx
	jg wrong_pass
	
	inc dword [sum]

wrong_pass:
	jmp line_start
	
read_num:
	xor eax, eax
	xor ebx, ebx
	next_num:
		lodsb
		
		cmp al, '0'   ; if (char<'0' || char>'9') ret
		jl if_not_char
		cmp al, '9'
		jle end_if_not_char
		if_not_char:
			ret
		end_if_not_char:
		
		sub al, '0'
		xchg eax, ebx
		mul ecx
		add eax, ebx
		xchg eax, ebx
		jmp next_num

