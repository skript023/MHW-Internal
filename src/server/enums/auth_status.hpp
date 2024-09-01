#pragma once

namespace big
{
	enum class eAuthStatus : uint32_t
	{
		UNAUTHENTICATED = 1 << 25,
		AUTHENTICATED = 1 << 12,
		AUTHENTICATION_FAILED = 1 << 16,
		ILLEGAL = 2 << 8
	};
}