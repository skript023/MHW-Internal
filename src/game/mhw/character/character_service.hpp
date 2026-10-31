#pragma once
#include "core/common.hpp"

#include "game/classes/fwddec.hpp"
#include <vector>
#include <string>

namespace big
{
	struct hunter_buff_data
	{
		std::string name;
		float timer = 0.0f;
		float max_timer = 180.0f;
		bool is_active = false;
		bool is_permanent = false;
		uint32_t color = 0xFFFFFFFF;
	};

	struct mantle_slot_data
	{
		int slot_id = 0;
		std::string name = "Mantle";
		float duration = 0.0f;
		float max_duration = 120.0f;
		float cooldown = 0.0f;
		float max_cooldown = 150.0f;
		bool is_active = false;
		bool is_on_cooldown = false;
	};

	class character_service
	{
		static character_service& get()
		{
			static character_service instance;
			return instance;
		}

		void init_impl(Character chara);
		void init_impl(PlayerPosition* coords);
		void init_impl(Waypoint* wp);
		void init_impl(PlayerForward* fwd);
		void set_player_entity_impl(uintptr_t entity);

		[[nodiscard]] std::string get_name_impl() const;
		[[nodiscard]] uint32_t get_zeni_impl() const;
		[[nodiscard]] uint32_t get_high_rank_level_impl() const;
		[[nodiscard]] uint32_t get_research_points_impl() const;
		[[nodiscard]] uint32_t get_high_rank_experience_impl() const;
		[[nodiscard]] uint32_t get_time_playing_impl() const;
		[[nodiscard]] uint32_t get_master_rank_level_impl() const;
		[[nodiscard]] uint32_t get_master_rank_experience_impl() const;
		[[nodiscard]] Vector3 get_position_impl() const;
		[[nodiscard]] Vector3 get_forward_pos_impl() const;
		[[nodiscard]] Vector3 get_waypoint_pos_impl() const;
		[[nodiscard]] uintptr_t get_player_entity_impl() const;

		[[nodiscard]] std::vector<hunter_buff_data> get_active_buffs_impl() const;
		[[nodiscard]] std::vector<mantle_slot_data> get_mantles_impl() const;

		void set_name_impl(std::string_view const& name);
		void set_zeni_impl(uint32_t zeni);
		void set_high_rank_level_impl(uint32_t level);
		void set_research_points_impl(uint32_t pts);
		void set_high_rank_experience_impl(uint32_t exp);
		void set_time_playing_impl(uint32_t time);
		void set_master_rank_level_impl(uint32_t level);
		void set_master_rank_experience_impl(uint32_t exp);
		void set_player_coords_impl(Vector3 vec3);
		void set_player_to_waypoint_impl();
		void set_player_forward_impl();

	public:
		explicit character_service() = default;
		~character_service() noexcept = default;

		static void init(Character chara)
		{
			get().init_impl(chara);
		}
		static void init(PlayerPosition* coords)
		{
			get().init_impl(coords);
		}
		static void init(Waypoint* wp)
		{
			get().init_impl(wp);
		}
		static void init(PlayerForward* fwd)
		{
			get().init_impl(fwd);
		}
		static void set_player_entity(uintptr_t entity)
		{
			get().set_player_entity_impl(entity);
		}

		[[nodiscard]] static std::string get_name()
		{
			return get().get_name_impl();
		}
		[[nodiscard]] static uint32_t get_zeni()
		{
			return get().get_zeni_impl();
		}
		[[nodiscard]] static uint32_t get_high_rank_level()
		{
			return get().get_high_rank_level_impl();
		}
		[[nodiscard]] static uint32_t get_research_points()
		{
			return get().get_research_points_impl();
		}
		[[nodiscard]] static uint32_t get_high_rank_experience()
		{
			return get().get_high_rank_experience_impl();
		}
		[[nodiscard]] static uint32_t get_time_playing()
		{
			return get().get_time_playing_impl();
		}
		[[nodiscard]] static uint32_t get_master_rank_level()
		{
			return get().get_master_rank_level_impl();
		}
		[[nodiscard]] static uint32_t get_master_rank_experience()
		{
			return get().get_master_rank_experience_impl();
		}
		[[nodiscard]] static Vector3 get_position()
		{
			return get().get_position_impl();
		}
		[[nodiscard]] static Vector3 get_forward_pos()
		{
			return get().get_forward_pos_impl();
		}
		[[nodiscard]] static Vector3 get_waypoint_pos()
		{
			return get().get_waypoint_pos_impl();
		}
		[[nodiscard]] static uintptr_t get_player_entity()
		{
			return get().get_player_entity_impl();
		}

		[[nodiscard]] static std::vector<hunter_buff_data> get_active_buffs()
		{
			return get().get_active_buffs_impl();
		}
		[[nodiscard]] static std::vector<mantle_slot_data> get_mantles()
		{
			return get().get_mantles_impl();
		}

		static void set_name(std::string_view const& name)
		{
			get().set_name_impl(name);
		}
		static void set_zeni(uint32_t zeni)
		{
			get().set_zeni_impl(zeni);
		}
		static void set_high_rank_level(uint32_t level)
		{
			get().set_high_rank_level_impl(level);
		}
		static void set_research_points(uint32_t pts)
		{
			get().set_research_points_impl(pts);
		}
		static void set_high_rank_experience(uint32_t exp)
		{
			get().set_high_rank_experience_impl(exp);
		}
		static void set_time_playing(uint32_t time)
		{
			get().set_time_playing_impl(time);
		}
		static void set_master_rank_level(uint32_t level)
		{
			get().set_master_rank_level_impl(level);
		}
		static void set_master_rank_experience(uint32_t exp)
		{
			get().set_master_rank_experience_impl(exp);
		}
		static void set_player_coords(Vector3 vec3)
		{
			get().set_player_coords_impl(vec3);
		}
		static void set_player_to_waypoint()
		{
			get().set_player_to_waypoint_impl();
		}
		static void set_player_forward()
		{
			get().set_player_forward_impl();
		}

	private:
		Character m_character{};
		PlayerPosition* m_position = nullptr;
		Waypoint* m_waypoint = nullptr;
		PlayerForward* m_forward = nullptr;
		uintptr_t m_player_entity = 0;
	};
}
