EXTERN g_affinity_addr: DWORD
EXTERN g_affinity_ret_addr: DQ
EXTERN g_affinity_call_addr: DQ

.CODE
	player_affinity_stat PROC
		call [g_affinity_call_addr]
		addss xmm0, [g_affinity_addr]
		movzx eax, byte ptr [rsp + 78h]
		jmp [g_affinity_ret_addr]
	player_affinity_stat ENDP
END