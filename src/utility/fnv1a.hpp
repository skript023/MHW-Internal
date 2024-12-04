#pragma once

inline constexpr uint32_t FNV1A_32_OFFSET_BASIS = 0x811C9DC5;
inline constexpr uint32_t FNV1A_32_PRIME = 0x01000193;

inline constexpr uint32_t fnv1a_32(const char* str, uint32_t hash = FNV1A_32_OFFSET_BASIS)
{
	return (*str == '\0') ? hash : fnv1a_32(str + 1, (hash ^ static_cast<uint32_t>(*str)) * FNV1A_32_PRIME);
}

inline constexpr uint32_t fnv1a_32(std::string_view str, uint32_t hash = FNV1A_32_OFFSET_BASIS)
{
	for (char c : str) {
		hash = (hash ^ static_cast<uint32_t>(c)) * FNV1A_32_PRIME;
	}
	return hash;
}

inline constexpr uint32_t operator"" _hash(const char* str, size_t)
{
	return fnv1a_32(str);
}