GLOBAL enterCritRegion
GLOBAL leaveCritRegion

	enterCritRegion:
		mov al,1
		XCHG al,[rdi]
		CMP al,0
		JNE mutexLock
		RET
	leaveCritRegion:
		mov [rdi],0
		RET