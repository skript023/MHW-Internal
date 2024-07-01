#include "common.hpp"
#include <byte_patching.hpp>

namespace big
{
    class mid_function_hook 
    {
    public:
        template <typename... Args>
        mid_function_hook(void* target_address, std::vector<byte> new_code, Args... args): 
            m_target_address(target_address), m_new_code(new_code), m_nop_count(0), m_is_nop(false)
        {
            (parse_args(std::forward<Args>(args)), ...);
        }

        void apply();

        void restore();

        ~mid_function_hook();

    private:
        void allocate();
        void initialize();
        void* allocate_executable_memory(void* target, size_t size);
        template <typename T>
        void parse_args(T arg)
        {
            if constexpr (std::is_same_v<T, float>)
            {
                std::vector<byte> floatBytes = floatToBytes(arg);
                m_new_code.insert(m_new_code.end(), floatBytes.begin(), floatBytes.end());
            }
            else if constexpr (std::is_same_v<T, float*>)
            {
                m_float_address = arg;

                m_new_code.resize(m_new_code.size() + sizeof(float), 0x00);
            }
            else if constexpr (std::is_same_v<T, int>)
            {
                if (m_is_nop)
                {
                    m_nop_count = arg; // Set NOP count if boolean flag is set
                }
                else
                {
                    std::vector<byte> intBytes = intToBytes(arg);
                    m_new_code.insert(m_new_code.end(), intBytes.begin(), intBytes.end());
                }
            }
            else if constexpr (std::is_same_v<T, int*>)
            {
                m_int_address = arg;

                m_new_code.resize(m_new_code.size() + sizeof(int), 0x00);
            }
            else if constexpr (std::is_same_v <T, std::vector<byte>>)
            {
                std::vector<byte> next(arg);

                m_new_code.insert(m_new_code.end(), next.begin(), next.end());
            }
            else if constexpr (std::is_same_v<T, bool>)
            {
                m_is_nop = arg;
            }
            // Add more argument types as needed
        }
        template <typename T, typename... Args>
        void parse_args(T arg, Args... args)
        {
            parse_args(arg);
            parse_args(args...);
        }
        // Helper function to convert a float to a byte array
        std::vector<byte> floatToBytes(float value);
        std::vector<byte> intToBytes(int value);

        bool m_is_nop;
        bool m_is_applied;

        int m_nop_count;

        void* m_value;
        void* m_target_address;
        void* m_int_address = nullptr;
        void* m_float_address = nullptr;
        void* m_new_code_address = nullptr;

        std::vector<byte> m_new_code;
        std::unique_ptr<byte[]> m_original_bytes;
        std::unique_ptr<byte_patching> m_patch;
    };
}