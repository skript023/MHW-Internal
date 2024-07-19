#include "hooking.hpp"
#include <services/character/character_service.hpp>

namespace big
{
	void __fastcall hooks::deploy_character(Character a1, uint32_t a2)
	{
		if (&a1)
		{
			g_character_service->init(a1);
		}

		return g_hooking->m_character_ptr_hook.get_original<decltype(&hooks::deploy_character)>()(a1, a2);
	}
}