#pragma once
#include <common.hpp>

struct Monster
{
	char padding[199];
	uint64_t m_material_count; //0xC8
	uint64_t m_body_count; //0XD0
};
