.DATA
	g_player_info DQ 0
	g_player_info_ret_addr DQ 0
.CODE
	player_info PROC
		mov [g_player_info],rdi
		mov ecx,[rdi+000000A0h]
		jmp [g_player_info_ret_addr]
	player_info ENDP
END