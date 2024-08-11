PUBLIC great_sword_ret_addr
.DATA
	great_sword_ret_addr DQ 0

.CODE
	great_sword_charge_asm PROC
		mov rax, 3
		mov [rbx + 2358h], rax
		jmp great_sword_ret_addr
	great_sword_charge_asm ENDP
END