#include "hooking.hpp"

namespace big
{
	void __fastcall hooks::consumable(Consumeable* rax, int a2, DWORD* a3)
	{
		if (g_settings->player.inf_item)
		{
            if (!rax) return g_hooking->m_consumable_hook.get_original<decltype(&hooks::consumable)>()(rax, a2, a3);

            auto r13 = rax->field_14; // Example interpretation of r13
            int r14d = -a2;

            if (r13 == (uintptr_t)rax)
            {
                return g_hooking->m_consumable_hook.get_original<decltype(&hooks::consumable)>()(rax, a2, a3);
            }

            switch (rax->field_08) 
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
                if (rax->field_08 > 0xCC)
                {
                    ++(rax->field_0C);
                }
                break;
            }

            uint32_t r12d = rax->field_0C;
            if (r14d == r12d) 
            {
                return g_hooking->m_consumable_hook.get_original<decltype(&hooks::consumable)>()(rax, a2, a3);
            }
		}

        return g_hooking->m_consumable_hook.get_original<decltype(&hooks::consumable)>()(rax, a2, a3);
	}
}