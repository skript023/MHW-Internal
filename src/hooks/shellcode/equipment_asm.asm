EXTERN g_equipment_ptr: DQ
EXTERN g_equipment_ret_addr: DQ

.CODE
	equipment PROC
		mov [g_equipment_ptr],r15
		movsx r8d,byte ptr [r15+2Ch]
		jmp [g_equipment_ret_addr + 5]
	equipment ENDP
END