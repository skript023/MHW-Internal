EXTERN g_frostcraft_sheathed_ret_addr: DQ
EXTERN g_frostcraft_sheathed: DWORD
EXTERN g_is_frostcraft_sheathed: BYTE

.CODE
	frostcraft_sheathed PROC
		cmp byte ptr [g_is_frostcraft_sheathed], 1
		jne return
		mulss xmm0,[g_frostcraft_sheathed]
	return:
		jmp g_frostcraft_sheathed_ret_addr
	frostcraft_sheathed ENDP
END