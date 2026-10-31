.CODE
	increase_pet_placement PROC
		mov rax, 1Eh
		mov [rbx + 244h], rax
		cmp dword ptr [rbx + 244h], 01h
		jmp ret_addr
	ret_addr:
		ret
	increase_pet_placement ENDP
END