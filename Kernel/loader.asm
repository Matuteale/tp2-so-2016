global loader
global callScheduler
extern main
extern initializeKernelBinary

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

ss_addr:
  resq 1

