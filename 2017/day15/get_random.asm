[BITS 64]

GLOBAL get_random
GLOBAL compare_low_16

SECTION .data

ALIGN 32
%include "vperm_lut.asm"    ; 8192 bytes

modulus:    dd 0x7FFFFFFF

SECTION .text



; uint32_t get_random(
;     uint32_t prev[64],   // rdi
;     int n,               // rsi
;     uint32_t *out,       // rdx
;     uint32_t multiplier, // rcx
;     uint32_t mask        // r8
; );
;
; Given the previous 64 numbers, test them by AND'ing with `mask` (to select multiples of 4 or 8),
; and write matching numbers contiguously into `out`.  Then multiply each number by `multiplier`,
; which should be 16807^64 or 48271^64 (mod 2^31-1) for generator A and B respectively.
;
; Repeat `n` times, testing a total of n*64 numbers and storing ~25% (gen A) or ~12.5% (gen B)
; of them.  Caller should pick `n` so that:
;     n * 64 * sizeof(uint32_t) * hit_percentage
; for each generator fits in cache.
;
; Returns: number of numbers returned in `out`.

get_random:
	add rcx, rcx
	movq xmm15, rcx
	vpbroadcastd ymm15, xmm15     ; ymm15 = [8x32] 2 * multiplier
	vpxor ymm14, ymm14, ymm14     ; ymm14 = all bits zero
	movq xmm13, r8
	vpbroadcastd ymm13, xmm13     ; ymm13 = [8x32] mask
	vpbroadcastd ymm12, [modulus] ; ymm12 = [8x32] 0x7FFFFFFF

	vmovdqu ymm0, [rdi]           ; ymm0..7 = previous 64 numbers
	vmovdqu ymm1, [rdi+32]
	vmovdqu ymm2, [rdi+64]
	vmovdqu ymm3, [rdi+96]
	vmovdqu ymm4, [rdi+128]
	vmovdqu ymm5, [rdi+160]
	vmovdqu ymm6, [rdi+192]
	vmovdqu ymm7, [rdi+224]

	shr rdx, 2
	mov rcx, rdx ; original out pointer

	mov r8, _vperm_lut_256x32
	mov r9, 8192

	.loop:
		%macro filter_mult 2
		; take all the multiples-of-X from %1, write them contiguously into [4*rdx], advance rdx
			vpand ymm8, %1, ymm13
			vpcmpeqd ymm8, ymm8, ymm14
			vmovmskps eax, ymm8
			shl eax, 5                 ; rax = offset into LUT
			vmovdqu ymm9, [%2 + rax]
			popcnt eax, eax
			vpermd ymm8, ymm9, %1
			vmovdqu [4*rdx], ymm8
			add rdx, rax

		; multiply %1 by multiplier (16807^64 for generator A, 48271^64 for B)
			vpmuludq ymm8, %1, ymm15     ; [high 31 bits, low 31 bits*]    *still left-shifted by 1
			vpsrlq %1, %1, 32            ; now take the even DWORDs
			vpmuludq %1, %1, ymm15       ; [high 31 bits, low 31 bits*]    *still left-shifted by 1
			vshufps ymm9, %1, ymm8, 0x22 ; LO_4, LO_2, LO_3, LO_1, LO_8, LO_6, LO_7, LO_5
			vshufps %1, %1, ymm8, 0x77   ; HI_4, HI_2, HI_3, HI_1, HI_8, HI_6, HI_7, HI_5
			vpsrld ymm9, 1               ; right-shift the low parts
			vpaddd ymm9, %1              ; modular reduction by 2^31-1
			vpsubd %1, ymm9, ymm12       ; 
			vpminud %1, %1, ymm9         ; the vpaddd may carry into the 32nd bit, reduce it again
		%endmacro

		filter_mult ymm0, r8
		filter_mult ymm1, r8
		filter_mult ymm2, r8
		filter_mult ymm3, r8
		filter_mult ymm4, r8
		filter_mult ymm5, r8
		filter_mult ymm6, r8
		filter_mult ymm7, r8

		; Because vshufps shuffles the numbers from {1,2,3,4,5,6,7,8} to {4,2,3,1,8,6,7,5},
		; every even iteration needs to undo this shuffle. We can do this cheaply by
		; having two vperm lookup tables. The following xor switches between these tables.
		xor r8, r9

	sub rsi, 1
	jnz .loop

	vmovdqu [rdi], ymm0
	vmovdqu [rdi+32], ymm1
	vmovdqu [rdi+64], ymm2
	vmovdqu [rdi+96], ymm3
	vmovdqu [rdi+128], ymm4
	vmovdqu [rdi+160], ymm5
	vmovdqu [rdi+192], ymm6
	vmovdqu [rdi+224], ymm7

	sub rdx, rcx
	mov rax, rdx

	ret



; uint32_t compare_low_16(
;     uint32_t *a,         // rdi
;     uint32_t *b,         // rsi
;     uint32_t count       // rdx
; );
;
; Returns:
;   rval = 0;
;   for (i=0; i<count; i++) rval += (a[i] & 0xFFFF) == (b[i] & 0xFFFF);

compare_low_16:
	lea r8, [rdi + 4 * rdx - 128] ; r8 = 128 bytes before end of a[]

	xor rax, rax

	cmp rdi, r8 ; less than 32 numbers / 128 bytes, don't do SIMD at all
	jg .finish

	.fastloop:
		vmovdqu ymm0, [rdi]
		vpcmpeqw ymm1, ymm0, [rsi]
		vpmovmskb rcx, ymm1
		and rcx, 0x11111111

		vmovdqu ymm0, [rdi + 32]
		vpcmpeqw ymm1, ymm0, [rsi + 32]
		vpmovmskb r10, ymm1
		and r10d, 0x22222222
		or rcx, r10

		vmovdqu ymm0, [rdi + 62]
		vpcmpeqw ymm1, ymm0, [rsi + 62]
		vpmovmskb r10, ymm1
		and r10d, 0x44444444
		or rcx, r10

		vmovdqu ymm0, [rdi + 94]
		vpcmpeqw ymm1, ymm0, [rsi + 94]
		vpmovmskb r10, ymm1
		and r10d, 0x88888888
		or rcx, r10

		popcnt rcx, rcx
		add rax, rcx

		add rdi, 128
		add rsi, 128
	cmp rdi, r8
	jle .fastloop
	
	.finish:
	add r8, 128 ; r8 = at end of a[]

	cmp rdi, r8
	je .done

	.slowloop:
		mov cx, [rdi]
		cmp cx, [rsi]
		jne .nocount
			inc rax
		.nocount:
		add rdi, 4
		add rsi, 4
	cmp rdi, r8
	jne .slowloop
	
	.done:
	ret
