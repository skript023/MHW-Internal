#include "hooking.hpp"

namespace big
{
	__int64 __fastcall hooks::high_rank_experience(__int64 a1, int a2)
	{
		__int64 v2; // rdi@1
		int v3; // esi@1
		char i; // bp@1
		int v5; // ecx@1
		int v6; // er15@3
		char v7; // al@3
		signed int v8; // er14@3
		__int64 v9; // rsi@12
		int v10; // eax@54
		unsigned int v11; // eax@60

		v2 = a1;
		v3 = a2;

		int exp = *(DWORD*)(v2 + 156) * g_settings->player.hr_ex_mult;
		v5 = v3 + exp;
		if ((unsigned int)v5 > 0x5F5E0FF)
			v5 = 99999999;
		*(DWORD*)(v2 + 156) = v5;

		return base_hook::get_original<hooks::high_rank_experience>()(a1, a2);
	}
}