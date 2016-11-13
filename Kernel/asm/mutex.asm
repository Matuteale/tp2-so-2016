GLOBAL enterCritRegion
GLOBAL leaveCritRegion

	enterCritRegion:
		mov al,1
		XCHG al,[rdi]
		CMP al,0
		JNE enterCritRegion
		RET
	leaveCritRegion:
		mov [rdi], BYTE 0
		RET