#include "hooking.hpp"

namespace big
{
	void __fastcall hooks::palico_experience_handler(__int64 a1, unsigned int a2, int a3, int a4)
	{
		__int64 v4; // rsi@1
		__int64 v5; // rdi@1
		int v6; // ebp@1
		int v7; // er14@1
		signed __int64 v8; // rcx@1
		unsigned int v9; // eax@1
		int v10; // eax@2

		v4 = a1;
		v5 = a2;
		v6 = a4;
		v7 = a3;
		v8 = v4 + 4 * v5;
		v9 = *(DWORD*)(v8 + 1058400);
		if (v9 < v6)
		{
			v10 = (v7 * g_settings->player.palico_exp_mult) + v9;
			if (v10 > (unsigned int)v6)
				v10 = v6;
			*(DWORD*)(v8 + 1058400) = v10;
		}

		return base_hook::get_original<hooks::palico_experience_handler>()(a1, a2, a3, a4);
	}
}