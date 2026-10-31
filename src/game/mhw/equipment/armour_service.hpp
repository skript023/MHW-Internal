#pragma once
#include "game/classes/fwddec.hpp"

namespace big
{
	class armour_service
	{
		static armour_service& get_service()
		{
			static armour_service instance;
			return instance;
		}

		void init_impl(Equipment* eq);
		Equipment* get_impl();

	public:
		explicit armour_service() = default;
		~armour_service() noexcept = default;

		static void init(Equipment* eq)
		{
			get_service().init_impl(eq);
		}

		static Equipment* get()
		{
			return get_service().get_impl();
		}

	private:
		Equipment* m_equipment = nullptr;
	};
}
