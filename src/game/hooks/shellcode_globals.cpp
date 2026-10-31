// Globals referenced by the MASM stubs in game/hooks/shellcode/*.asm. They keep
// the shellcode's exact symbol names, so they live in the global namespace.
#include <cstdint>

extern "C"
{
	float g_frostcraft_drawn;
	float g_frostcraft_sheathed;
	uint64_t g_equipment_ptr;
}
