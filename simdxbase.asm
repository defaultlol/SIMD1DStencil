section .text
default rel
bits 64
global SIMDXVersionStencil
;Paulo Delgado
SIMDXVersionStencil:
	push rbp
	mov rbp, rsp
	mov rsi, rdx ; Ptr for Y
	mov rdi, rcx ; Ptr for X
	mov rcx, r8 ; ARRAY_SIZE
	xor rax,rax
	xor r9,r9
	add rax,3
	add r9,2

loop_start:
	cmp r9,r8
	jnle loop_end
	vmovdqu xmm1, [rdi+rax*8]
	vmovdqu xmm2, [rdi+(rax-3)*8]
	vmovdqu xmm3, [rdi+(rax-2)*8]
	vmovdqu xmm4, [rdi+(rax-1)*8]
	vmovdqu xmm5, [rdi+(rax+3)*8]
	vmovdqu xmm6, [rdi+(rax+2)*8]
	vmovdqu xmm7, [rdi+(rax+1)*8]
	vaddpd xmm0,xmm1,xmm2
	vaddpd xmm0,xmm0,xmm3
	vaddpd xmm0,xmm0,xmm4
	vaddpd xmm0,xmm0,xmm5
	vaddpd xmm0,xmm0,xmm6
	vaddpd xmm0,xmm0,xmm7
	vmovupd [rsi],xmm0
	add rsi,16
	add rax,2
	add r9,2
	jmp loop_start

loop_end:
	pop rbp
	ret