#pragma once
#include "base.hpp"

namespace big
{
	class detour_hook : public base_hook
	{
		void fix_hook_address();
	public:
		explicit detour_hook(const std::string_view name, void* target, void* detour);
		~detour_hook() noexcept;

		bool enable();
		bool disable();

		void enable_immediately() const;
		void disable_immediately() const;

		void* get_original_ptr();

		template <typename T>
		inline T get_original()
		{
			return static_cast<T>(get_original_ptr());
		}

		template <auto T>
		static void add(std::string name, void* target)
		{
			base_hook::add<T>(new detour_hook(name, target, T));
		}
	private:
		void* m_target;
		void* m_detour;
		void* m_original{};
	};
}
