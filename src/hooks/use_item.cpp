#include "hooking.hpp"

namespace big
{
	void __fastcall hooks::use_item(__int64* rcx, unsigned char r8l)
	{
		if (g_settings->player.inf_item)
		{
            // Custom hook logic based on original assembly code
            if (*(rcx + 0x08) > 0xCC)
                goto code2;
            if (r8l != 0x00)
                goto end;

            goto return2;

        code2:
            if (r8l != 0x01)
                goto end;

        return2:
            return g_hooking->m_use_item_hook.get_original<decltype(&hooks::use_item)>()(rcx, r8l);

        end:
            return g_hooking->m_use_item_hook.get_original<decltype(&hooks::use_item)>()(rcx, r8l);
		}

        return g_hooking->m_use_item_hook.get_original<decltype(&hooks::use_item)>()(rcx, r8l);
	}
}