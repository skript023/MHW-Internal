#include "hook_manager.hpp"
#include "pointers.hpp"

namespace big
{
	hook_manager::hook_manager() :
		/*mid function hook*/
		m_easy_breaker(g_pointers->m_easy_breaker, { 0x48, 0xC7, 0x41, 0x10, 0x00, 0x00, 0x00, 0x00 }),
		m_part_breaker(g_pointers->m_part_breaker, { 0xC7, 0x84, 0x24, 0xA8, 0x01, 0x00, 0x00 }, 990.f, true, 5),
		m_reveal_monster(g_pointers->m_reveal_monster, { 0xC7, 0x47, 0x28, 0x00, 0x00, 0xC8, 0x42, 0xF3, 0x0F, 0x10, 0x77, 0x28 }),
		m_maxed_effect_set(g_pointers->m_maxed_effect_set, { 0xC6, 0x40, 0x31, 0x05, 0x0F, 0xB6, 0x48, 0x31, 0x00, 0x8C, 0xD3, 0x80, 0x00, 0x00, 0x00 }, true, 6),
		m_maxed_attribute(g_pointers->m_maxed_attribute, { 0x43 , 0xC6 , 0x44 , 0x34 , 0x31 , 0x07 , 0x43 , 0x0F , 0xB6 , 0x44 , 0x34 , 0x31 }, true, 1),
		m_max_sharpness(g_pointers->m_max_sharpness, { 0xC7, 0x81 ,0xF8 ,0x20 ,0x00 ,0x00 ,0x90 ,0x01 ,0x00 ,0x00 ,0x8B ,0xB9 ,0xF8 ,0x20 ,0x00 ,0x00 }, true, 1),
		m_gs_inst_chrgs(g_pointers->m_gs_inst_chrgs, { 0xC7, 0x83, 0x58, 0x23, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00 }, true, 1),
		m_mount_reset(g_pointers->m_mount_reset, { 0xC7 ,0x81 ,0x70 ,0xC1 ,0x01 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0xF3 ,0x0F ,0x10 ,0x81 ,0x70 ,0xC1 ,0x01 ,0x00 }, true, 3),
		m_frostcraft_drawn(g_pointers->m_frostcraft_drawn, { 0xF3, 0x0F, 0x59, 0x05 }, &g_settings->weapon.frostcraft_drawn),
		m_frostcraft_heat(g_pointers->m_frostcraft_heat, { 0xF3, 0x0F, 0x59, 0x05 }, &g_settings->weapon.frostcraft_heat, true, 4),
		m_lucky_voucher(g_pointers->m_lucky_voucher, { 0xC7, 0x80, 0x54, 0x91, 0x26, 0x00, 0x05, 0x00, 0x00, 0x00, 0x3B, 0xB0, 0x54, 0x91, 0x26, 0x00 }, true, 1),
		/*byte patching*/
		m_player_state(g_pointers->m_player_state, { 0x90, 0x90, 0x90, 0x90, 0x90 }),
		m_player_anim(g_pointers->m_player_anim, { 0x90, 0x90, 0x90, 0x90, 0x90 }),
		m_player_health(g_pointers->m_player_health, { 0x90, 0x90, 0x90, 0x90, 0x90 }),
		m_player_stamina(g_pointers->m_player_stamina, { 0x90, 0x90, 0x90, 0x90 }),
		m_inf_mount(g_pointers->m_inf_mount, { 0x90, 0x90 }),
		m_inf_frostcraft(g_pointers->m_inf_frostcraft, { 0x90, 0x90, 0x90, 0x90 }),
		m_gathering_resource(g_pointers->m_gathering_resource, { 0x90, 0x90 })
	{
		g_hook_manager = this;
	}

	hook_manager::~hook_manager()
	{
		/*mid function hook*/
		m_easy_breaker.restore();
		m_part_breaker.restore();
		m_reveal_monster.restore();
		m_maxed_effect_set.restore();
		m_maxed_attribute.restore();
		m_max_sharpness.restore();
		m_gs_inst_chrgs.restore();
		m_mount_reset.restore();
		m_frostcraft_drawn.restore();
		m_frostcraft_heat.restore();
		m_lucky_voucher.restore();
		/*byte patching*/
		m_player_anim.restore();
		m_player_health.restore();
		m_player_state.restore();
		m_player_stamina.restore();
		m_inf_mount.restore();
		m_inf_frostcraft.restore();
		m_gathering_resource.restore();

		g_hook_manager = nullptr;
	}
}