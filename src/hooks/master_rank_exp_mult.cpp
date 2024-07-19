#include "hooking.hpp"

namespace big
{
	__int64 __fastcall hooks::master_rank_experience(__int64 a1, uint32_t a2, char a3)
	{
		__int64 v3; // rsi@1
		char v4; // di@1
		unsigned int v5; // er15@1
		int v7; // ecx@3

		v3 = a1;
		v4 = a3;
		v5 = a2;

		auto exp = g_settings->player.mr_ex_mult * *(DWORD*)(v3 + 216);
		v7 = v5 + exp;              // EXP HR ADD
		if ((unsigned int)v7 > 0x5F5E0FF)
			v7 = 99999999;
		*(DWORD*)(v3 + 216) = v7;

		return g_hooking->m_master_rank_mult_hook.get_original<decltype(&hooks::master_rank_experience)>()(a1, a2, a3);
	}
}