#pragma once
#include "data/monster_data.hpp"
#include "data/monster_weakness_data.hpp"
#include "mhw/fwddec.hpp"

#include <vector>
#include <string>
#include <cstdint>

namespace big
{
	struct monster_part_info
	{
		int id = 0;
		std::string name;
		float health = 0.0f;
		float max_health = 0.0f;
		uint32_t break_count = 0;
		bool is_severable = false;
		bool is_severed = false;
		int slash_hzv = 0;
		int blunt_hzv = 0;
		int shot_hzv = 0;
		int elem_hzv = 0;
		std::string elem_type = "Dragon";
		bool is_weakpoint = false;
	};

	struct monster_ailment_info
	{
		int id = 0;
		std::string name;
		float buildup = 0.0f;
		float max_buildup = 100.0f;
		float duration = 0.0f;
		float max_duration = 0.0f;
		bool is_active = false;
		uint32_t color = 0xFFFFFFFF;
	};

	class monster_service
	{
		static monster_service& get()
		{
			static monster_service instance;
			return instance;
		}

		bool get_entry_impl(int index);
		_NODISCARD float get_health_impl(int index);
		_NODISCARD float get_max_health_impl(int index);
		_NODISCARD std::string get_monster_name_impl(int index);
		_NODISCARD int32_t get_monster_id_impl(int index);
		_NODISCARD monster_data::monster_combat_profile get_combat_profile_impl(int index);
		_NODISCARD float get_timer_impl(int index);
		_NODISCARD float get_max_timer_impl(int index);
		_NODISCARD float get_size_impl(int index);
		_NODISCARD Vector3 get_position_impl(int index);

		void register_parts_manager_impl(uintptr_t manager_ptr, unsigned int part_index);
		[[nodiscard]] std::vector<monster_part_info> get_parts_impl(int index);
		[[nodiscard]] std::vector<monster_ailment_info> get_ailments_impl(int index);

	public:
		explicit monster_service();
		~monster_service() noexcept = default;

		static bool get_entry(int index)
		{
			return get().get_entry_impl(index);
		}
		_NODISCARD static float get_health(int index)
		{
			return get().get_health_impl(index);
		}
		_NODISCARD static float get_max_health(int index)
		{
			return get().get_max_health_impl(index);
		}
		_NODISCARD static std::string get_monster_name(int index)
		{
			return get().get_monster_name_impl(index);
		}
		_NODISCARD static int32_t get_monster_id(int index)
		{
			return get().get_monster_id_impl(index);
		}
		_NODISCARD static monster_data::monster_combat_profile get_combat_profile(int index)
		{
			return get().get_combat_profile_impl(index);
		}
		_NODISCARD static float get_timer(int index)
		{
			return get().get_timer_impl(index);
		}
		_NODISCARD static float get_max_timer(int index)
		{
			return get().get_max_timer_impl(index);
		}
		_NODISCARD static float get_size(int index)
		{
			return get().get_size_impl(index);
		}
		_NODISCARD static Vector3 get_position(int index)
		{
			return get().get_position_impl(index);
		}

		static void register_parts_manager(uintptr_t manager_ptr, unsigned int part_index)
		{
			get().register_parts_manager_impl(manager_ptr, part_index);
		}
		[[nodiscard]] static std::vector<monster_part_info> get_parts(int index)
		{
			return get().get_parts_impl(index);
		}
		[[nodiscard]] static std::vector<monster_ailment_info> get_ailments(int index)
		{
			return get().get_ailments_impl(index);
		}

	private:
		std::vector<MonsterEntry*> m_entry = {nullptr, nullptr, nullptr};
		uintptr_t m_parts_manager[3] = {0, 0, 0};
		float m_ailment_buildup[3][6] = {};
	};
}
