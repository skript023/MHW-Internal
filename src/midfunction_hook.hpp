#include "common.hpp"
#include <byte_patching.hpp>

namespace big
{
    class mid_function_hook 
    {
    public:
        // Constructor with vector of bytes
        mid_function_hook(void* target_address, std::vector<byte> new_code);

        // Constructor with nop bytes
        mid_function_hook(void* target_address, std::vector<byte> new_code, int nop);

        // Constructor with float data
        mid_function_hook(void* target_address, const std::vector<byte>& new_code, float data);

        void apply() const;

        void restore() const;

        ~mid_function_hook();

    private:
        void initialize();
        void* allocate_executable_memory(void* target, size_t size);

        // Helper function to convert a float to a byte array
        std::vector<byte> floatToBytes(float value);

        int m_nop;
        void* m_value;
        void* m_target_address;
        std::unique_ptr<byte[]> m_original_bytes;
        void* m_new_code_address;
        std::vector<byte> m_new_code;
        std::unique_ptr<byte_patching> m_patch;
    };
}