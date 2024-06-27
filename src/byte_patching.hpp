#include <common.hpp>

namespace big
{
    // Define a concept for types compatible with std::span
    template<typename T>
    concept SpanCompatibleType = requires(T a) { std::span{ a }; };

    // Byte patch class
    class byte_patching 
    {
    public:
        byte_patching(void* address, const std::vector<byte>& bytes);

        ~byte_patching();

        void apply() const;

        void restore() const;
    private:
        void* m_address;
        std::unique_ptr<byte[]> m_value;
        std::unique_ptr<byte[]> m_original_bytes;
        std::size_t m_size;
        DWORD m_old_protect;
    };
}