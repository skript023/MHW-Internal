#pragma once
#include "data/monster_data.hpp"
#include "mhw/fwddec.hpp"

namespace big
{
	class monster_service
	{
	public:
		explicit monster_service();

		~monster_service() noexcept;

		bool get_entry(int index);
		_NODISCARD float get_health(int index);
		_NODISCARD float get_max_health(int index);
		_NODISCARD std::string get_monster_name(int index);
		_NODISCARD float get_timer(int index);
		_NODISCARD float get_max_timer(int index);
		_NODISCARD float get_size(int index);
	private:
		std::vector<MonsterEntry*> m_entry = { nullptr, nullptr, nullptr };
	};

	inline monster_service* g_monster_service{};
}