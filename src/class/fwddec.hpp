#pragma once
#include <common.hpp>

struct Monster
{
	char padding[199];
	uint64_t m_material_count; //0xC8
	uint64_t m_body_count; //0XD0
};
static_assert(sizeof(Monster) == 0xD8);

struct PlayerStat
{
	char m_pad0x18F[399];
	float m_frostcraft_recharges_drawn; //0x190
	char padding[1556]; //0x194
	float m_critical_boost; //0x7A8
};
static_assert(sizeof(PlayerStat) == 0x7AC);