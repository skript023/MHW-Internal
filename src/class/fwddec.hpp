#pragma once
#include <common.hpp>

class Monster
{
public:
	char padding[199];
	uint64_t m_material_count; //0xC8
	uint64_t m_body_count; //0XD0
};
static_assert(sizeof(Monster) == 0xD8);

class PlayerStat
{
public:
	char m_pad0x18F[399];
	float m_frostcraft_recharges_drawn; //0x190
	char padding[1556]; //0x194
	float m_critical_boost; //0x7A8
};
static_assert(sizeof(PlayerStat) == 0x7AC);

class Consumeable
{
public:
	uint32_t field_04; // corresponds to [rax + 0x04]
	uint32_t field_08; // corresponds to [rax + 0x08]
	uint32_t field_0C; // corresponds to [rax + 0x0C]
	uint32_t field_10; // corresponds to [rax + 0x10]
	uintptr_t field_14; // corresponds to [rax + 0x14]
};
static_assert(sizeof(Consumeable) == 0x18);

class Equipment
{
public:
	char pad_0000[11]; //0x00
	int8_t m_num_active_slot; //0x0B
	char pad_000C[8]; //0x0C
	short m_id_layered; //0x14
	short m_defense; //0x16
	char pad_0018[6]; //0x18
	short m_id_ed; //0x1E
	char pad_0020[12]; //0x20
	int8_t m_fire_res; //0x2C
	int8_t m_water_res; //0x2D
	int8_t m_thunder_res; //0x2E
	int8_t m_ice_res; //0x2F
	int8_t m_dragon_res; //0x30
	int8_t m_bonus_level; //0x31
	char pad_0032; //0x32
	int8_t m_skill_level_a; //0x33
	int8_t m_skill_level_b; //0x34
	int8_t m_skill_level_c; //0x35
	int8_t m_set_bonus; //0x36
	char m_pad_0037[3]; //0x37
	int8_t m_skill_a; //0x3A
	char pad_003B; //0x3B
	int8_t m_skill_b; //0x3C
	char pad_003D; //0x3D
	int8_t m_skill_c; //0x3E
	int8_t m_slot_a_level; //0x40
	int8_t m_slot_b_level; //0x41
	int8_t m_slot_c_level; //0x42
};
static_assert(sizeof(Equipment) == 0x42);