global loader
global callScheduler
extern main
extern initializeKernelBinary
extern schedule

%macro pusha 0
    push    rax
    push    rbx
    push    rcx
    push    rdx
    push    rbp
    push    rdi
    push    rsi
    push    r8
    push    r9
    push    r10
    push    r11
    push    r12
    push    r13
    push    r14
    push    r15
    push    fs
    push    gs
%endmacro

%macro popa 0
    pop     gs
    pop     fs
    pop     r15
    pop     r14
    pop     r13
    pop     r12
    pop     r11
    pop     r10
    pop     r9
    pop     r8
    pop     rsi
    pop     rdi
    pop     rbp
    pop     rdx
    pop     rcx
    pop     rbx
    pop     rax
%endmacro

loader:
	call initializeKernelBinary	;Set up the kernel binary,get thet stack address
	mov rsp, rax				;Set up the stack with the returned address
	call main

hang:
	hlt							;halt machine should kernel return
	jmp hang


section .bss


  ret_addr:
    resq 1

  cs_addr:
    resq 1

  ss_addr:
    resq 1
