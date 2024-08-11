#include "hooking.hpp"

namespace big
{
	__int64 hooks::monster_on_map(__int64 a1, int a2, int a3)
	{
		if (g_settings->player.reveal_monster)
		{
			auto v3 = *(DWORD*)(a1 + 264);
			auto v4 = v3 ? *(uintptr_t*)(a1 + 280) : 0ui64;
			auto v5 = v3 ? v4 + 8 * v3 : 0ui64;
			auto v6 = v3 ? *(uintptr_t*)(a1 + 280) + 8 * v3 : 0ull;

			if (v4 != v6 && v4 != 0)
			{
				auto v7 = *(uintptr_t*)v4;

				*(float*)(v7 + 40) = 100.f;
			}

			return 3;
		}

		return g_hooking->m_monster_on_map_hook.get_original<decltype(&hooks::monster_on_map)>()(a1, a2, a3);
	}
}