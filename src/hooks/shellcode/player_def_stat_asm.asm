EXTERN g_def_rate_addr: DWORD
EXTERN g_def_addr: DWORD
EXTERN g_def_ret_addr: QWORD

.CODE
	player_def_stat PROC
		movss xmm6,[g_def_addr]
		add edi, [g_def_addr + 04h]
		sub r8d, ecx
		cmp r8d, edi
		jmp [g_def_ret_addr]
	player_def_stat ENDP
END