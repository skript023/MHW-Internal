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
        m_original_bytes = std::make_unique<byte[]>(5);  // Assuming 5 bytes for the jump
        memcpy(m_original_bytes.get(), m_target_address, 5);

        // Calculate the return address where the original function should resume
        uintptr_t return_address = (uintptr_t)m_target_address + 5;

        // Add the return jump to the end of the new code
        m_new_code.push_back(0xE9); // Add the jmp opcode
        uintptr_t relative_offset = return_address - ((uintptr_t)m_new_code.data() + m_new_code.size() + 4);
        m_new_code.insert(m_new_code.end(), (byte*)&relative_offset, (byte*)&relative_offset + 4);

        // Allocate memory for new code near target address
        m_new_code_address = allocate_executable_memory(m_target_address, m_new_code.size() + 0x1024);

        if (!m_new_code_address)
        {
            throw std::runtime_error("Failed to allocate memory for hook.");
        }

        // Copy the new code to allocated memory
        memcpy(m_new_code_address, m_new_code.data(), m_new_code.size());

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

        LOG(HACKER) << "Return address: " << "0x" << std::hex << return_address;
        LOG(HACKER) << "Jump address: " << "0x" << std::hex << relative_address;
        LOG(HACKER) << "Allocated address: " << "0x" << std::hex << m_new_code_address;
        LOG(HACKER) << "Target address: " << "0x" << std::hex << m_target_address;

        // Patch the original function with the jump to the new code
        m_patch = std::make_unique<byte_patching>(m_target_address, jmp_newmem);
    }
    void* mid_function_hook::allocate_executable_memory(void* target, size_t size)
    {
        SYSTEM_INFO sysInfo;
        GetSystemInfo(&sysInfo);
        uintptr_t startAddress = (uintptr_t)target - 0x7FFFFF00;  // 2GB lower
        uintptr_t endAddress = (uintptr_t)target + 0x7FFFFF00;    // 2GB higher

        uintptr_t address = startAddress;
        while (address < endAddress)
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
}