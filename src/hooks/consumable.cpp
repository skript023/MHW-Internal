#include "hooking.hpp"

namespace big
{
	void __fastcall hooks::consumable(__int64 a1, int a2, DWORD* a3)
	{
		if (g_settings->player.inf_item)
		{
            int v1 = a2;
            if (a2 < 0)
            {
                // Prevent decrementing the item count by setting a2 to 0
                a2 = 0;
            }

            // Optionally, you can set the item count to a specific value
            // For example, set it to 99 to ensure it stays at a high value
            if (a2 > 0)
            {
                a2 = 99;
            }
            
            return g_hooking->m_consumable_hook.get_original<decltype(&hooks::consumable)>()(a1, a2, a3);
		}

        return g_hooking->m_consumable_hook.get_original<decltype(&hooks::consumable)>()(a1, a2, a3);
	}
}