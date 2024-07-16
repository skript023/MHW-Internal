.DATA
	monreamultv DD 0
	ret_addr DQ 0
.CODE
	research_experience_asm PROC
		mulss xmm6, monreamultv
		addss xmm0, xmm6
		shr r15, 05h
		jmp ret_addr
	research_experience_asm ENDP
END