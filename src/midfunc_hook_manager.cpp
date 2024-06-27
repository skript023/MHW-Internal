#include "midfunc_hook_manager.hpp"
#include "pointers.hpp"

namespace big
{
	midfunc_hook_manager::midfunc_hook_manager() :
		m_easy_breaker(g_pointers->m_easy_breaker, { 0x48, 0xC7, 0x41, 0x10, 0x00, 0x00, 0x00, 0x00 }),
		m_part_breaker(g_pointers->m_part_breaker, { 0xC7, 0x84, 0x24, 0xA8, 0x01, 0x00, 0x00 }, 990.f),
		m_reveal_monster(g_pointers->m_reveal_monster, { 0xC7, 0x84, 0x24, 0xA8, 0x01, 0x00, 0x00 }, 100.f)
	{
		g_midfunction_manager = this;
	}

	midfunc_hook_manager::~midfunc_hook_manager()
	{
		m_easy_breaker.restore();
		m_part_breaker.restore();
		m_reveal_monster.restore();

		g_midfunction_manager = nullptr;
	}
}