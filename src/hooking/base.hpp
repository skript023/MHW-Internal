#pragma once

namespace big
{
	class base_hook
	{
	private:

	protected:
		const std::string_view m_name;
		bool m_enabled;
	public:
		base_hook(const std::string_view name);
		virtual ~base_hook() = default;
		base_hook(const base_hook&) = delete;
		base_hook(base_hook&&) noexcept = delete;
		base_hook& operator=(const base_hook&) = delete;
		base_hook& operator=(base_hook&&) noexcept = delete;

		const std::string_view name() const
		{
			return m_name;
		}
		inline bool is_enabled() const
		{
			return m_enabled;
		}

		virtual bool enable() = 0;
		virtual bool disable() = 0;
		virtual void* get_original_ptr() = 0;

	public:
		template<auto detour_function>
		struct detour_helper
		{
			static inline base_hook* m_hook;
		};

		template<auto detour_function>
		static void add(base_hook* hook);

		template<auto detour_function, typename T>
		static T* get();

		template<auto detour_function>
		static auto get_original();

		static std::vector<base_hook*>& hooks();

		static bool enable_all();
		static bool disable_all();

	private:
		inline static std::vector<base_hook*> m_base_hooks;

	};

	template<auto detour_function>
	inline void base_hook::add(base_hook* hook)
	{
		detour_helper<detour_function>::m_hook = hook;
	}

	template<auto detour_function, typename T>
	inline T* base_hook::get()
	{
		return reinterpret_cast<T*>(detour_helper<detour_function>::m_hook);
	}

	template<auto detour_function>
	inline auto base_hook::get_original()
	{
		return reinterpret_cast<decltype(detour_function)>(detour_helper<detour_function>::m_hook->get_original_ptr());
	}
}