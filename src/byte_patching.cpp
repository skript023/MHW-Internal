#include "byte_patching.hpp"

namespace big
{
    byte_patching::byte_patching(void* address, const std::vector<byte>& bytes)
        : m_address(address), m_size(bytes.size()), m_original_bytes(nullptr), m_old_protect(0)
    {
        m_original_bytes = std::make_unique<byte[]>(m_size);
        memcpy(m_original_bytes.get(), m_address, m_size);

        m_value = std::make_unique<byte[]>(m_size);
        memcpy(m_value.get(), bytes.data(), m_size);
    }
    byte_patching::~byte_patching()
    {
        restore();
    }
    void byte_patching::apply() const
    {
        if (m_is_applied)
            return;

        DWORD oldProtect;
        VirtualProtect(m_address, m_size, PAGE_EXECUTE_READWRITE, &oldProtect);
        memcpy(m_address, m_value.get(), m_size);
        VirtualProtect(m_address, m_size, oldProtect, &oldProtect);

        memset(const_cast<bool*>(&m_is_applied), true, sizeof(bool));
    }
    void byte_patching::restore() const
    {
        if (!m_is_applied)
            return;

        DWORD oldProtect;
        VirtualProtect(m_address, m_size, PAGE_EXECUTE_READWRITE, &oldProtect);
        memcpy(m_address, m_original_bytes.get(), m_size);
        VirtualProtect(m_address, m_size, oldProtect, &oldProtect);

        memset(const_cast<bool*>(&m_is_applied), false, sizeof(bool));
    }
}