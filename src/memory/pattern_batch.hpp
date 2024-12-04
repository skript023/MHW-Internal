#pragma once
#include <functional>
#include <vector>
#include "pattern.hpp"
#include "pattern_cache.hpp"

namespace memory
{
	class pattern_batch
	{
	public:
		pattern_batch() = default;
		explicit pattern_batch(std::shared_ptr<pattern_cache> pattern_cache);
		~pattern_batch() noexcept;

		void add(std::string name, pattern pattern, std::function<void(memory::handle)> callback);
		void run(range region);
		void update()
		{
			m_pattern_cache->update();
		}
	
		struct entry
		{
			std::string m_name;
			pattern m_pattern;
			pattern_hash m_hash;
			std::function<void(memory::handle)> m_callback;

			explicit entry(std::string name, pattern pattern, std::function<void(memory::handle)> callback) :
				m_name(std::move(name)),
				m_pattern(std::move(pattern)),
				m_callback(std::move(callback)),
				m_hash(m_pattern.m_hash)
			{}
		};
	
	private:
		std::vector<entry> m_entries;
		std::shared_ptr<pattern_cache> m_pattern_cache;
	};
}
