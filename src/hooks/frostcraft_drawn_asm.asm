EXTERN g_frostcraft_drawn_ret_addr: DQ
EXTERN g_frostcraft_drawn: DWORD
EXTERN g_is_frostcraft_drawn: BYTE

.CODE
	frostcraft_drawn PROC
		cmp byte ptr [g_is_frostcraft_drawn], 1
		jne return
		mulss xmm0,[g_frostcraft_drawn]
	return:
		jmp g_frostcraft_drawn_ret_addr
	frostcraft_drawn ENDP
END