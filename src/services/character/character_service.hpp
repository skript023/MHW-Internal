#pragma once
#include "common.hpp"
#include <class/fwddec.hpp>

namespace big
{
	class character_service
	{
	public:
		explicit character_service();
		~character_service() noexcept;

		void init(Character chara);
		void init(PlayerPosition* coords);
		void init(Waypoint* wp);
		void init(PlayerForward* fwd);

		[[nodiscard]] std::string get_name() const;
		[[nodiscard]] uint32_t get_zeni() const;
		[[nodiscard]] uint32_t get_high_rank_level() const;
		[[nodiscard]] uint32_t get_research_points() const;
		[[nodiscard]] uint32_t get_high_rank_experience() const;
		[[nodiscard]] uint32_t get_time_playing() const;
		[[nodiscard]] uint32_t get_master_rank_level() const;
		[[nodiscard]] uint32_t get_master_rank_experience() const;
		[[nodiscard]] Vector3 get_position() const;
		[[nodiscard]] Vector3 get_waypoint_pos() const;
		
		void set_name(std::string_view const& name);
		void set_zeni(uint32_t zeni);
		void set_high_rank_level(uint32_t level);
		void set_research_points(uint32_t pts);
		void set_high_rank_experience(uint32_t exp);
		void set_time_playing(uint32_t time);
		void set_master_rank_level(uint32_t level);
		void set_master_rank_experience(uint32_t exp);
		void set_player_coords(Vector3 vec3);
		void set_player_to_waypoint();
		void set_player_forward();
	private:
		Character m_character;
		PlayerPosition* m_position = nullptr;
		Waypoint* m_waypoint = nullptr;
		PlayerForward* m_forward = nullptr;
	};

	inline character_service* g_character_service{};
}