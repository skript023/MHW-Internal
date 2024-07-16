EXTERN g_player_info: DQ
EXTERN g_player_info_ret_addr: DQ

.CODE
	player_info PROC
		mov [g_player_info],rdi
		mov ecx,[rdi+000000A0h]
		jmp [g_player_info_ret_addr]
	player_info ENDP
END