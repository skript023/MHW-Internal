#include "midfunction_hook.hpp"

namespace big
{
    mid_function_hook::~mid_function_hook()
    {
        restore();
    }
    void mid_function_hook::apply()
    {
        if (m_allocated_float)
            *(float*)m_allocated_float = *(float*)m_float_address;

        if (!m_new_code_address)
        {
            m_new_code_address = allocate_executable_memory(m_target_address, m_new_code.size() + 0x1024);

            if (!m_new_code_address)
            {
                throw std::runtime_error("Failed to allocate memory for hook.");
            }

            if (m_float_address)
            {
                m_allocated_float = allocate_executable_memory((void*)((uintptr_t)m_new_code_address + 1024), sizeof(float) + 1);
                memcpy(m_allocated_float, m_float_address, sizeof(void*));

                uintptr_t relative_float_address = (uintptr_t)m_allocated_float - ((uintptr_t)m_new_code_address + sizeof(uint64_t));
                std::vector<byte> float_address_bytes = intToBytes(relative_float_address);
                m_new_code[4] = (byte)(relative_float_address & 0xFF);
                m_new_code[5] = (byte)((relative_float_address >> 8) & 0xFF);
                m_new_code[6] = (byte)((relative_float_address >> 16) & 0xFF);
                m_new_code[7] = (byte)((relative_float_address >> 24) & 0xFF);

                LOG(INFO) << "Float address : " << std::hex << m_float_address;
                LOG(INFO) << "Float rel address : " << std::hex << (uintptr_t)m_float_address - ((uintptr_t)m_new_code_address);
            }

            if (m_int_address)
            {
                uintptr_t relative_int_address = (uintptr_t)m_int_address - ((uintptr_t)m_new_code_address);
                std::vector<byte> int_address_bytes = intToBytes(relative_int_address);
                m_new_code.insert(m_new_code.end(), int_address_bytes.begin(), int_address_bytes.end());
            }

            // Copy the new code to allocated memory
            memcpy(m_new_code_address, m_new_code.data(), m_new_code.size());

            // Add jump back to the original function after new code execution
            uintptr_t return_address = (uintptr_t)m_target_address + 5;
            byte* jump_back_location = (byte*)m_new_code_address + m_new_code.size();
            jump_back_location[0] = 0xE9; // jmp opcode

            // Calculate relative offset for jump back to the original code
            uintptr_t relative_offset = return_address - ((uintptr_t)jump_back_location + 5);
            memcpy(jump_back_location + 1, &relative_offset, 4);

            // Prepare the jump to the new code
            uintptr_t relative_address = (uintptr_t)m_new_code_address - ((uintptr_t)m_target_address + 5);

            // Patch the target address with the jump
            std::vector<byte> jmp_newmem = {
                0xE9, // jump instruction
                (byte)(relative_address & 0xFF),
                (byte)((relative_address >> 8) & 0xFF),
                (byte)((relative_address >> 16) & 0xFF),
                (byte)((relative_address >> 24) & 0xFF)
            };

            // Add nop if is nop true
            if (m_is_nop && m_nop_count > 0)
            {
                jmp_newmem.insert(jmp_newmem.end(), m_nop_count, 0x90);
            }

            // Patch the original function with the jump to the new code
            m_patch = std::make_unique<byte_patching>(m_target_address, jmp_newmem);
        }

        m_patch->apply();
    }
    void mid_function_hook::restore()
    {
        if (m_patch)
        {
            m_patch->restore();
        }

        if (m_new_code_address)
        {
            VirtualFree(m_new_code_address, 0, MEM_RELEASE);
            m_new_code_address = nullptr;
        }

        if (m_allocated_float)
        {
            VirtualFree(m_allocated_float, 0, MEM_RELEASE);
            m_allocated_float = nullptr;
        }
    }
    void mid_function_hook::initialize()
    {
        
    }
    void* mid_function_hook::allocate_executable_memory(void* target, size_t size)
    {
        SYSTEM_INFO sysInfo;
        GetSystemInfo(&sysInfo);
        uintptr_t start = (uintptr_t)target - 0x7FFFFF00;  // 2GB lower
        uintptr_t end = (uintptr_t)target + 0x7FFFFF00;    // 2GB higher

        uintptr_t address = start;
        while (address < end)
        {
            if (address > (uintptr_t)target)
            {
                void* allocated = VirtualAlloc((void*)address, size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

                if (allocated)
                {
                    return allocated;
                }
            }

            address += sysInfo.dwPageSize;
        }

        throw std::runtime_error("Failed to allocate memory near target address.");
    }
    std::vector<byte> mid_function_hook::floatToBytes(float value)
    {
        std::vector<byte> bytes(sizeof(value));
        memcpy(bytes.data(), &value, sizeof(value));
        return bytes;
    }
    std::vector<byte> mid_function_hook::intToBytes(int value)
    {
        std::vector<byte> bytes(sizeof(value));
        memcpy(bytes.data(), &value, sizeof(value));
        return bytes;
    }
}