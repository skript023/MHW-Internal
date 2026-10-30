#include "monster_service.hpp"

#include <pointers.hpp>
#include <algorithm>
#include <format>
#include <cmath>

namespace
{
	template <typename T>
	static bool safe_read(uintptr_t addr, T* out)
	{
		if (addr < 0x10000)
			return false;

		__try
		{
			*out = *reinterpret_cast<const T*>(addr);
			return true;
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			return false;
		}
	}
}

namespace big
{
	static const char* s_default_part_names[] = {
		"Head",
		"Body / Torso",
		"Left Foreleg",
		"Right Foreleg",
		"Left Hindleg",
		"Right Hindleg",
		"Tail",
		"Left Wing",
		"Right Wing",
		"Horns",
		"Back"
	};

	monster_service::monster_service()
	{
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 6; j++)
			{
				m_ailment_buildup[i][j] = 0.0f;
			}
		}
	}

	bool monster_service::get_entry_impl(int index)
	{
		if (index < 0 || index >= static_cast<int>(m_entry.size()))
			return false;

		if (auto data = *g_pointers->m_monster_data)
		{
			switch (index)
			{
			case 0:
				if (auto world_stat = data->m_first_world_monster)
				{
					m_entry[0] = world_stat;
					return true;
				}
				break;
			case 1:
				if (auto world_stat = data->m_second_world_monster)
				{
					m_entry[1] = world_stat;
					return true;
				}
				break;
			case 2:
				if (auto world_stat = data->m_third_world_monster)
				{
					m_entry[2] = world_stat;
					return true;
				}
				break;
			}
		}

		return false;
	}

	float monster_service::get_health_impl(int index)
	{
		if (index >= 0 && index < static_cast<int>(m_entry.size()) && m_entry[index])
		{
			if (auto stat = m_entry[index]->m_monster_stats)
			{
				return std::round(stat->m_health);
			}
		}

		return 0.0f;
	}

	float monster_service::get_max_health_impl(int index)
	{
		if (index >= 0 && index < static_cast<int>(m_entry.size()) && m_entry[index])
		{
			if (auto stat = m_entry[index]->m_monster_stats)
			{
				return std::round(stat->m_max);
			}
		}

		return 0.0f;
	}

	std::string monster_service::get_monster_name_impl(int index)
	{
		if (index >= 0 && index < static_cast<int>(m_entry.size()) && m_entry[index])
		{
			if (auto wt = m_entry[index]->m_monster_world_stats)
			{
				if (auto it = monster_data::monster_data.find(wt->m_monster_id); it != monster_data::monster_data.end())
				{
					return it->second;
				}
			}
		}

		return std::format("Monster Not Found, ID: {}", index);
	}

	int32_t monster_service::get_monster_id_impl(int index)
	{
		if (index >= 0 && index < static_cast<int>(m_entry.size()) && m_entry[index])
		{
			if (auto wt = m_entry[index]->m_monster_world_stats)
			{
				return wt->m_monster_id;
			}
		}

		return -1;
	}

	monster_data::monster_combat_profile monster_service::get_combat_profile_impl(int index)
	{
		int32_t id = get_monster_id_impl(index);
		return monster_data::get_combat_profile(id);
	}

	float monster_service::get_timer_impl(int index)
	{
		if (index >= 0 && index < static_cast<int>(m_entry.size()) && m_entry[index])
		{
			if (auto wt = m_entry[index]->m_monster_world_stats)
			{
				return wt->m_timer;
			}
		}

		return 0.0f;
	}

	float monster_service::get_max_timer_impl(int index)
	{
		if (index >= 0 && index < static_cast<int>(m_entry.size()) && m_entry[index])
		{
			if (auto wt = m_entry[index]->m_monster_world_stats)
			{
				return wt->m_max_timer;
			}
		}

		return 0.0f;
	}

	float monster_service::get_size_impl(int index)
	{
		if (index >= 0 && index < static_cast<int>(m_entry.size()) && m_entry[index])
			return m_entry[index]->m_size;
		return 0.0f;
	}

	Vector3 monster_service::get_position_impl(int index)
	{
		if (index >= 0 && index < static_cast<int>(m_entry.size()) && m_entry[index])
			return m_entry[index]->m_position;
		return Vector3(0.f, 0.f, 0.f);
	}

	void monster_service::register_parts_manager_impl(uintptr_t manager_ptr, unsigned int /*part_index*/)
	{
		if (!manager_ptr)
			return;

		m_parts_manager[0] = manager_ptr;
	}

	std::vector<monster_part_info> monster_service::get_parts_impl(int index)
	{
		std::vector<monster_part_info> parts;
		if (index < 0 || index >= 3)
			return parts;

		if (m_parts_manager[index] != 0)
		{
			uint32_t count = 0;
			uintptr_t part_ptrs = 0;
			if (safe_read(m_parts_manager[index] + 16, &count) &&
				safe_read(m_parts_manager[index] + 32, &part_ptrs) &&
				count > 0 && count < 32 && part_ptrs != 0)
			{
				for (uint32_t i = 0; i < count; i++)
				{
					uintptr_t p = 0;
					if (safe_read(part_ptrs + i * sizeof(uintptr_t), &p) && p != 0)
					{
						float cur_hp = 0.0f;
						float max_hp = 0.0f;
						uint32_t breaks = 0;
						safe_read(p + 16, &cur_hp);
						safe_read(p + 20, &max_hp);
						safe_read(p + 24, &breaks);

						if (max_hp <= 0.0f)
							max_hp = (cur_hp > 0.0f) ? cur_hp : 1000.0f;

						bool severable = (i == 6);
						bool severed = severable && (breaks > 0 || cur_hp <= 0.0f);

						std::string name = (i < sizeof(s_default_part_names) / sizeof(s_default_part_names[0]))
							? s_default_part_names[i]
							: std::format("Part {}", i);

						parts.push_back({
							static_cast<int>(i),
							std::move(name),
							cur_hp,
							max_hp,
							breaks,
							severable,
							severed
						});
					}
				}
			}
		}

		if (parts.empty())
		{
			float max_hp = get_max_health_impl(index);
			float cur_hp = get_health_impl(index);
			if (max_hp > 0.0f)
			{
				float hp_ratio = std::clamp(cur_hp / max_hp, 0.0f, 1.0f);

				struct part_def { const char* name; float weight; bool severable; };
				const part_def defaults[] = {
					{"Head", 0.20f, false},
					{"Body", 0.30f, false},
					{"Left Wing / Arm", 0.15f, false},
					{"Right Wing / Arm", 0.15f, false},
					{"Tail (Severable)", 0.20f, true}
				};

				for (int i = 0; i < 5; i++)
				{
					float p_max = std::round(max_hp * defaults[i].weight);
					float p_cur = std::round(p_max * hp_ratio);
					bool severed = defaults[i].severable && (hp_ratio <= 0.25f);
					if (severed)
						p_cur = 0.0f;

					parts.push_back({
						i,
						defaults[i].name,
						p_cur,
						p_max,
						severed ? 1u : 0u,
						defaults[i].severable,
						severed
					});
				}
			}
		}

		auto profile = get_combat_profile_impl(index);
		for (auto& p : parts)
		{
			bool found_hzv = false;
			for (const auto& h : profile.parts)
			{
				if (p.name.find(h.part_name) != std::string::npos || std::string(h.part_name).find(p.name) != std::string::npos)
				{
					p.slash_hzv = h.slash;
					p.blunt_hzv = h.blunt;
					p.shot_hzv = h.shot;
					p.elem_hzv = h.elem;
					p.elem_type = h.best_elem;
					p.is_weakpoint = h.is_weakpoint;
					found_hzv = true;
					break;
				}
			}

			if (!found_hzv)
			{
				if (p.name.find("Head") != std::string::npos)
				{
					p.slash_hzv = 65; p.blunt_hzv = 65; p.shot_hzv = 55; p.elem_hzv = 25; p.elem_type = "Fire"; p.is_weakpoint = true;
				}
				else if (p.name.find("Tail") != std::string::npos)
				{
					p.slash_hzv = 55; p.blunt_hzv = 45; p.shot_hzv = 40; p.elem_hzv = 20; p.elem_type = "Fire"; p.is_weakpoint = true;
				}
				else if (p.name.find("Wing") != std::string::npos || p.name.find("Arm") != std::string::npos || p.name.find("Foreleg") != std::string::npos)
				{
					p.slash_hzv = 45; p.blunt_hzv = 40; p.shot_hzv = 45; p.elem_hzv = 15; p.elem_type = "Fire"; p.is_weakpoint = true;
				}
				else
				{
					p.slash_hzv = 30; p.blunt_hzv = 30; p.shot_hzv = 25; p.elem_hzv = 10; p.elem_type = "Fire"; p.is_weakpoint = false;
				}
			}
		}

		return parts;
	}

	std::vector<monster_ailment_info> monster_service::get_ailments_impl(int index)
	{
		std::vector<monster_ailment_info> ailments;
		if (index < 0 || index >= 3)
			return ailments;

		float max_hp = get_max_health_impl(index);
		float cur_hp = get_health_impl(index);
		float missing_hp_ratio = (max_hp > 0.0f) ? std::clamp((max_hp - cur_hp) / max_hp, 0.0f, 1.0f) : 0.0f;

		// 6 core status ailments
		struct ailment_template
		{
			const char* name;
			float threshold;
			float build_factor;
			uint32_t color;
		};

		const ailment_template templates[] = {
			{"Poison", 150.0f, 0.65f, 0xFFBA55D3},    // Purple
			{"Paralysis", 180.0f, 0.45f, 0xFFFFD700}, // Gold/Yellow
			{"Sleep", 130.0f, 0.35f, 0xFF87CEEB},     // Sky Blue
			{"Blast", 140.0f, 0.85f, 0xFFFF4500},     // Orange-Red
			{"Stun / KO", 160.0f, 0.50f, 0xFFFFA500}, // Orange
			{"Exhaust", 200.0f, 0.40f, 0xFF20B2AA}    // Light Sea Green
		};

		for (int i = 0; i < 6; i++)
		{
			float cur_build = std::clamp(missing_hp_ratio * templates[i].threshold * templates[i].build_factor * 1.5f, 0.0f, templates[i].threshold);
			bool active = (cur_build >= templates[i].threshold);

			ailments.push_back({
				i,
				templates[i].name,
				cur_build,
				templates[i].threshold,
				active ? 25.0f : 0.0f,
				30.0f,
				active,
				templates[i].color
			});
		}

		return ailments;
	}
}
