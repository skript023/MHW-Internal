#include "armour_service.hpp"

namespace big
{
	armour_service::armour_service()
	{
		g_armour_service = this;
	}

	armour_service::~armour_service() noexcept
	{
		g_armour_service = nullptr;
	}

	void armour_service::init(Equipment* eq)
	{
		m_equipment = eq;
	}
	Equipment* armour_service::get()
	{
		if (!m_equipment)
			return nullptr;

		return m_equipment;
	}
}