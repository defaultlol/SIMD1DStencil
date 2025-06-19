section .text
default rel
bits 64
global SIMDYVersionStencil
;Paulo Delgado
SIMDYVersionStencil:
	push rbp
	mov rbp, rsp
	mov rsi, rdx ; Ptr for Y
	mov rdi, rcx ; Ptr for X
	mov rcx, r8 ; ARRAY_SIZE
	xor rax,rax
	xor r9,r9
	add rax,3
	add r9,4

loop_start:
	cmp r9,r8
	jnle loop_end
	vmovdqu ymm1, [rdi+rax*8]
	vmovdqu ymm2, [rdi+(rax-3)*8]
	vmovdqu ymm3, [rdi+(rax-2)*8]
	vmovdqu ymm4, [rdi+(rax-1)*8]
	vmovdqu ymm5, [rdi+(rax+3)*8]
	vmovdqu ymm6, [rdi+(rax+2)*8]
	vmovdqu ymm7, [rdi+(rax+1)*8]
	vaddpd ymm0,ymm1,ymm2
	vaddpd ymm0,ymm0,ymm3
	vaddpd ymm0,ymm0,ymm4
	vaddpd ymm0,ymm0,ymm5
	vaddpd ymm0,ymm0,ymm6
	vaddpd ymm0,ymm0,ymm7
	vmovupd [rsi],ymm0
	add rsi,32
	add rax,4
	add r9,4
	jmp loop_start

loop_end:
	pop rbp
	ret