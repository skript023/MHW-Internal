#include "hooking.hpp"
#include "pointers.hpp"
#include <services/equipment/armour_service.hpp>

namespace big
{
	int __fastcall hooks::highlighted_armour(__int64 a1, __int64 a2, char a3, signed int* a4)
	{
		auto v6 = *(uint32_t*)(a2 + 32);
		auto v9 = *(uint32_t*)(a2 + 36);
		Equipment* v10 = g_pointers->m_select_armour(v6, v9);

		if (v10)
		{
			g_armour_service->init(v10);
		}

		return g_hooking->m_equipment_hook.get_original<decltype(&hooks::highlighted_armour)>()(a1, a2, a3, a4);
	}
}