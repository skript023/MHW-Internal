#include "hooking.hpp"

namespace big
{
	__int64 __fastcall hooks::infinite_gathering(__int64 a1, __int64 a2)
	{
		if (g_settings->player.inf_gathering)
		{
			auto temp = *(DWORD64*)(a1 + 0xC8);
			*(DWORD64*)(a1 + 0xC8) = 0;
			*(DWORD64*)(a1 + 0xD0) = temp;
			temp = 0;
		}

		return g_hooking->m_inf_gathering_hook.get_original<decltype(&infinite_gathering)>()(a1, a2);
	}
}