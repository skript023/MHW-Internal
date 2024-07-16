EXTERN g_atk_addr: DWORD
EXTERN g_atk_ret_addr: DQ

.CODE
	player_atk_stat PROC
		addss xmm6, [g_atk_addr]
		mov rdi, [rbx + 08h]
		xorps xmm9, xmm9
		jmp [g_atk_ret_addr]
	player_atk_stat ENDP
END