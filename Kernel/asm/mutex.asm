GLOBAL enterCritRegion
GLOBAL leaveCritRegion

	enterCritRegion:
		mov al,1
		XCHG al,[rdi]
		CMP al,0
		JNE enterCritRegionRejected
		RET
	leaveCritRegion:
		mov [rdi], BYTE 0
		RET

	enterCritRegionRejected:
		mov rax, 1
		RET