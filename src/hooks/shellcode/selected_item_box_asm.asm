EXTERN g_set_item: DWORD
EXTERN g_item_box: DQ
EXTERN g_selected_item_ret_addr: DQ

.CODE
	selected_item_box PROC
		mov eax, 0ffffffffh
		cmp [g_set_item], eax
		je label_return
		mov ecx, [rbx + 0Ch]
		cmp rcx, 0h
		jle label_return
		cmp [g_set_item], ecx
		jl label_return
		mov ecx, [g_set_item]
		mov [rbx + 0Ch], ecx
	label_return:
		mov [g_item_box], rbx
		mov edx, [rbx + 08h]
		mov rcx, rsi
		jmp g_selected_item_ret_addr
	selected_item_box ENDP
END