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

callScheduler: ;Guardo el contexto del proceso y llamo al scheduler

  pop     QWORD[ret_addr]       ;Direccion de retorno

  mov     QWORD[ss_addr], ss    ;Stack Segment
  push    QWORD[ss_addr]

  push      rsp

  pushf                         ;Se pushean los flags
  mov     QWORD[cs_addr], cs    ;Code Segment
  push    QWORD[cs_addr]
  push    QWORD[ret_addr]       ;Direccion de retorno

  pusha

  mov     rdi, rsp
  call    schedule ;Llama al scheduler avisa al scheduler que se guardó el contexto del proceso (le pasa el stack)
  mov     rsp, rax

  popa

  iretq

section .bss

  
ret_addr:
    resq 1
cs_addr:
    resq 1

cont_addr:
ss_addr:
    resq 1