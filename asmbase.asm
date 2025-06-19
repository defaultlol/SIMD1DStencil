section .text
default rel
bits 64
global ASMVersionStencil
ASMVersionStencil:
	push rbp
    mov rbp,rsp
    mov rsi,rdx            
    mov rdi,rcx          
    mov rcx,r8             
    xor rax,rax            
    xor r9,r9
    add rax,3
loop_start:
    cmp rax,rcx            
    je loop_end
    xor r10, r10
    MOV r10,qword [rdi + rax*8] ; i 
    ADD r10,qword [rdi + (rax-3)*8] ; i -3 
    ADD r10,qword [rdi + (rax-2)*8] ; i - 2
    ADD r10,qword [rdi + (rax-1)*8] ; i - 1
    ADD r10,qword [rdi + (rax+3)*8] ; i+3
    ADD r10,qword [rdi + (rax+2)*8]; i+2
    ADD r10,qword [rdi + (rax+1)*8]; i+1
    MOV qword [rsi + r9*8],r10 
    inc rax
    inc r9
    jmp loop_start
loop_end:
    pop rbp
    ret