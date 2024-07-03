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

struct Consumeable
{
	uint64_t padding; //0x00 
	uint64_t field_08; // corresponds to [rax + 0x08]
	uint32_t field_0C; // corresponds to [rax + 0x0C]
	uint32_t field_10; // corresponds to [rax + 0x10]
	uintptr_t field_14; // corresponds to [rax + 0x14]
};
static_assert(sizeof(Consumeable) == 0x18);