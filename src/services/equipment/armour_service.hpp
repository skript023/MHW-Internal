#pragma once
#include "mhw/fwddec.hpp"

namespace big
{
	class armour_service
	{
	public:
		explicit armour_service();
		~armour_service() noexcept;

		void init(Equipment* eq);
		Equipment* get();
	private:
		Equipment* m_equipment = nullptr;
	};

	inline armour_service* g_armour_service{};
}