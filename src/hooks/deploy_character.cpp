#include "hooking.hpp"
#include <services/character/character_service.hpp>

namespace big
{
	void __fastcall hooks::deploy_character(Character* a1, uint32_t a2)
	{
		std::shared_ptr<character_service> character;
		if (a1)
		{
			character = std::make_shared<character_service>(a1);
		}
		else
		{
			character.reset();
		}

		return g_hooking->m_character_ptr_hook.get_original<decltype(&hooks::deploy_character)>()(a1, a2);
	}
}