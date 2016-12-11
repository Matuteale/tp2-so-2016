GLOBAL enterCritRegion
GLOBAL leaveCritRegion

	enterCritRegion:
		mov al,1
		XCHG al,[rdi]
		cmp al,0
		jne enterCritRegionRejected
		ret
	leaveCritRegion:
		mov [rdi], BYTE 0
		ret

	enterCritRegionRejected:
		mov rax, 1
		ret