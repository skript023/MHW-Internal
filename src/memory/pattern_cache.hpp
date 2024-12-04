#pragma once
#include "pattern_hash.hpp"

namespace memory
{
	class pattern_cache
	{
		bool m_initialized;
		std::string m_name;
		std::unordered_map<std::uint64_t, int> m_data;
	public:

		pattern_cache(std::string const& name);

		void update();

		std::optional<int> get_cached_offset(pattern_hash hash);

		void update_cached_offset(pattern_hash hash, int offset);

		bool is_initialized() const
		{
			return m_initialized;
		}
	};
}