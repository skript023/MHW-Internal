#include "hooking.hpp"
#include <pointers.hpp>

namespace big
{
	void __fastcall hooks::consumable(__int64 a1, int a2, DWORD* a3)
	{
		if (g_settings->player.inf_item)
		{
			//uint32_t r13 = rax->field_14; // Example interpretation of r13
			uint32_t r14d = static_cast<uint32_t>(a2);

			//if (!rax) return g_hooking->m_consumable_hook.get_original<decltype(&hooks::consumable)>()(rax, a2, a3);
			LOG(INFO) << "A1 : " << std::hex << a1;
			LOG(INFO) << "Address A1 : " << &a1;
			LOG(INFO) << "A2 : " << (__int64)a2;
			LOG(INFO) << "A3 : " << (__int64)a3;

			const auto rax = std::make_unique<Consumeable>();

			switch (a1) 
			{
			case 0x69:
			case 0x3E8:
			case 0x3F0:
				rax->field_0C = 5;
				LOG(INFO) << "Executed wff";
				break;
			case 0xAF:
				rax->field_0C = 51;
				LOG(INFO) << "Executed pcw";
				break;
			case 0xB0:
			case 0xB1:
			case 0xB2:
			case 0xB4:
				rax->field_0C = 21;
				LOG(INFO) << "Executed ccw";
				break;
			case 0x3AC:
			case 0x3AD:
			case 0x3AE:
			case 0x3AF:
			case 0xA75:
			case 0xA76:
				rax->field_0C = 11;
				LOG(INFO) << "Executed efw";
				break;
			default:
				if (a1 > 0xCC)
				{
					++(rax->field_0C);
				}
				break;
			}

			return g_pointers->m_use_item(rax.get(), rax->field_0C, 1);

			uint32_t r12d = rax->field_0C;
			if (r14d == r12d) 
			{
				return g_hooking->m_consumable_hook.get_original<decltype(&hooks::consumable)>()(a1, a2, a3);
			}
		}

		return g_hooking->m_consumable_hook.get_original<decltype(&hooks::consumable)>()(a1, a2, a3);
	}
}