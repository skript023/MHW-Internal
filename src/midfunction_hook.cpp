#include "midfunction_hook.hpp"

namespace big
{
	mid_function_hook::mid_function_hook(void* target_address, std::vector<byte> new_code)
        : m_target_address(target_address), m_new_code(new_code)
    {
        initialize();
    }
    mid_function_hook::mid_function_hook(void* target_address, const std::vector<byte>& new_code, float data)
        : m_target_address(target_address)
    {
        std::vector<byte> floatBytes = floatToBytes(data);
        m_new_code = new_code;
        m_new_code.insert(m_new_code.end(), floatBytes.begin(), floatBytes.end());
        initialize();
    }
    mid_function_hook::~mid_function_hook()
    {
        restore();
        if (m_new_code_address)
        {
            VirtualFree(m_new_code_address, 0, MEM_RELEASE);
        }
    }
    void mid_function_hook::apply() const
    {
        m_patch->apply();
    }
    void mid_function_hook::restore() const
    {
        m_patch->restore();
    }
    void mid_function_hook::initialize()
    {
        m_original_bytes = std::make_unique<byte[]>(5);  // Original 5 bytes to be replaced by jump
        memcpy(m_original_bytes.get(), m_target_address, 5);

        // Allocate memory for new code
        m_new_code_address = VirtualAlloc(nullptr, m_new_code.size() + 5, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
        if (!m_new_code_address) 
        {
            throw std::runtime_error("Failed to allocate memory for hook.");
        }

        // Copy the new code to allocated memory
        memcpy(m_new_code_address, m_new_code.data(), m_new_code.size());

        // Add jump back to the original function after new code execution
        uintptr_t return_address = (uintptr_t)m_target_address + 5;
        *(byte*)((uintptr_t)m_new_code_address + m_new_code.size()) = 0xE9;
        *(uintptr_t*)((uintptr_t)m_new_code_address + m_new_code.size() + 1) = return_address - (uintptr_t)m_new_code_address - m_new_code.size() - 5;

        // Prepare the jump to the new code
        std::vector<byte> jmp_newmem = { 0xE9 };
        uintptr_t new_code_offset = (uintptr_t)m_new_code_address - (uintptr_t)m_target_address - 5;
        jmp_newmem.insert(jmp_newmem.end(), (byte*)&new_code_offset, (byte*)&new_code_offset + 4);

        // Patch the target address with the jump
        m_patch = std::make_unique<byte_patching>(m_target_address, jmp_newmem);
    }
    std::vector<byte> mid_function_hook::floatToBytes(float value)
    {
        std::vector<byte> bytes(sizeof(value));
        memcpy(bytes.data(), &value, sizeof(value));
        return bytes;
    }
}