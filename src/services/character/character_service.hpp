#pragma once
#include "common.hpp"
#include <class/fwddec.hpp>

namespace big
{
	class character_service
	{
	public:
		explicit character_service(Character* chara);
		~character_service() noexcept;

		[[nodiscard]] std::string get_name() const;
		[[nodiscard]] uint32_t get_zeni() const;
		[[nodiscard]] uint32_t get_high_rank_level() const;
		[[nodiscard]] uint32_t get_research_points() const;
		[[nodiscard]] uint32_t get_high_rank_experience() const;
		[[nodiscard]] uint32_t get_time_playing() const;
		[[nodiscard]] uint32_t get_master_rank_level() const;
		[[nodiscard]] uint32_t get_master_rank_experience() const;
		
		void set_name(std::string_view name);
		void set_zeni(uint32_t zeni);
		void set_high_rank_level(uint32_t level);
		void set_research_points(uint32_t pts);
		void set_high_rank_experience(uint32_t exp);
		void set_time_playing(uint32_t time);
		void set_master_rank_level(uint32_t level);
		void set_master_rank_experience(uint32_t exp);
	private:
		std::shared_ptr<Character> m_character;
	};

	inline character_service* g_character_service{};
}