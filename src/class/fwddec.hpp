#pragma once
#include <common.hpp>

class GatheringMonster
{
public:
	char padding[199];
	uint64_t m_material_count; //0xC8
	uint64_t m_body_count; //0XD0
};
static_assert(sizeof(GatheringMonster) == 0xD8);

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

class PlayerParams
{
public:
	uint32_t m_base; //0x00
	char pad_0004[180]; //0x04
	float m_gravity; //0xB8
	float m_gravity_speed_rate; //0xBC
	char pad_00C0[600]; //0xC0
	float m_health_initial; //0x318
	float m_health_max; //0x31C
	float m_health_recover_ratio; //0x320
	float m_health_recover_delay; //0x324
	float m_health_recover_interval; //0x328
	float m_health_recover_amount; //0x32C
	float m_stamina_initial; //0x330
	float m_stamina_max; //0x334
	float m_stamina_tired; //0x338
	float m_stamina_recover; //0x33C
	float m_stamina_regen_rate; //0x340
	float m_stamina_reduction_eat; //0x344
	float m_stamina_reduction_timer; //0x348
	float m_stamina_escape_dash_cost; //0x34C
	float m_stamina_cost_rate; //0x350
	float unk_354; //0x354
	float m_stamina_multiplier_action; //0x358
	float m_stamina_multiplier_bow; //0x35C
	float m_stamina_cost_action; //0x360
	char pad_364[12]; //0x364
	float m_dp_cost; //0x370
	char pad_0374[14]; //0x374
	float unk_388; //0x388
	char pad_38C[212]; //0x38C
	float m_bow_charge_step; //0x460
};
static_assert(sizeof(PlayerParams) == 0x460, "PlayerParams not properly sized.");

class MonsterWorldStatus
{
public:
	char pad_0000[120328]; //0x0000
	float m_timer; //0x1D608
	float m_max_timer; //0x1D60C
	char pad_1D610[424]; //0x1D610
	uint32_t m_monster_id; //0x1D7B8
};
static_assert(sizeof(MonsterWorldStatus) == 0x1D7BC, "MonsterWorldStatus not properly sized.");

class MonsterStats
{
public:
	char pad_0000[96];
	float m_max;
	float m_health;
};
static_assert(sizeof(MonsterStats) == 0x68, "MonsterStats not properly sized.");

class MonsterEntry
{
public:
	char pad_0000[392]; //0x0000
	float m_size; //0x0188
	char pad_018C[29924]; //0x018C
	MonsterStats* m_monster_stats; //0x7670
	char pad_7678[44032]; //0x7678
	MonsterWorldStatus* m_monster_world_stats; //0x12278
};
static_assert(sizeof(MonsterEntry) == 0x12280, "MonsterEntry not properly sized.");

class Monster
{
public:
	char pad_0000[3672];
	MonsterEntry* m_first_world_monster; //0xE58
	char pad_0E60[72];
	MonsterEntry* m_second_world_monster; //0xEA8
	char pad_0EB0[72];
	MonsterEntry* m_third_world_monster; //0xEF8
};
static_assert(sizeof(Monster) == 0xF00, "Monster not properly sized.");