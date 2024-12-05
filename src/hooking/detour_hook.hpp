#pragma once
#include "base.hpp"

namespace big
{
	class detour_hook : public base_hook
	{
	public:
		explicit detour_hook(const std::string_view name, void* target, void* detour);
		~detour_hook() noexcept;

		bool enable();
		bool disable();

		void enable_immediately() const;
		void disable_immediately() const;

		void* get_original_ptr();

		template <typename T>
		T get_original();

		void fix_hook_address();
	private:
		void* m_target;
		void* m_detour;
		void* m_original{};
	};

	template <typename T>
	inline T detour_hook::get_original()
	{
		return static_cast<decltype(T)>(get_original_ptr());
	}
}
