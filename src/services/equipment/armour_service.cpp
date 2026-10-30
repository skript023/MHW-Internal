#include "armour_service.hpp"

namespace big
{
	void armour_service::init_impl(Equipment* eq)
	{
		m_equipment = eq;
	}

	Equipment* armour_service::get_impl()
	{
		if (!m_equipment)
			return nullptr;

		return m_equipment;
	}
}
