EXTERN g_gs_charge_ret_addr: DQ

.CODE
	gs_inst_charge PROC
		mov rax, 3
		mov [rbx + 2358h], rax
		jmp g_gs_charge_ret_addr
	gs_inst_charge ENDP
END