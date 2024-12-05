#include "hooking.hpp"
#include <pointers.hpp>

namespace big
{
	void __fastcall hooks::consumable(__int64 a1, int a2, DWORD* a3)
	{
		if (g_settings->player.inf_item)
		{
			const auto rax = std::make_unique<Consumeable>();

			switch (a1) 
			{
			case 0x69:
			case 0x3E8:
			case 0x3F0:
				rax->field_0C = 5;
				break;
			case 0xAF:
				rax->field_0C = 51;
				break;
			case 0xB0:
			case 0xB1:
			case 0xB2:
			case 0xB4:
				rax->field_0C = 21;
				break;
			case 0x3AC:
			case 0x3AD:
			case 0x3AE:
			case 0x3AF:
			case 0xA75:
			case 0xA76:
				rax->field_0C = 11;
				break;
			default:
				if (a1 > 0xCC)
				{
					++(rax->field_0C);
				}
				break;
			}

			return g_pointers->m_use_item(rax.get(), rax->field_0C, 1);
		}

		return base_hook::get_original<hooks::consumable>()(a1, a2, a3);
	}
}