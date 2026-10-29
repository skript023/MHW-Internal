#pragma once

namespace big
{
	struct ImageDimensions
	{
		int x{0};
		int y{0};
		ImageDimensions() = default;
		ImageDimensions(int _x, int _y) : x(_x), y(_y) {}
	};
}
