#include "common.hpp"
#include <byte_patching.hpp>

namespace big
{
    class mid_function_hook 
    {
    public:
        template <typename... Args>
        mid_function_hook(void* target_address, std::vector<byte> new_code, Args... args)
            : m_target_address(target_address), m_new_code(new_code), m_nop(0), m_is_nop(false)
        {
            (add_args_to_new_code(std::forward<Args>(args)), ...);
            initialize();
        }

        void apply() const;

        void restore() const;

        ~mid_function_hook();

    private:
        void initialize();
        void* allocate_executable_memory(void* target, size_t size);
        template <typename T>
        void add_args_to_new_code(T arg)
        {
            if constexpr (std::is_same_v<T, float>)
            {
                std::vector<byte> floatBytes = floatToBytes(arg);
                m_new_code.insert(m_new_code.end(), floatBytes.begin(), floatBytes.end());
            }
            else if constexpr (std::is_same_v<T, int>)
            {
                if (m_is_nop)
                {
                    m_nop = arg; // Set NOP count if boolean flag is set
                }
                else
                {
                    std::vector<byte> intBytes = intToBytes(arg);
                    m_new_code.insert(m_new_code.end(), intBytes.begin(), intBytes.end());
                }
            }
            else if constexpr (std::is_same_v<T, bool>)
            {
                m_is_nop = arg;
            }
            // Add more argument types as needed
        }
        template <typename T, typename... Args>
        void add_args_to_new_code(T arg, Args... args)
        {
            add_args_to_new_code(arg);
            add_args_to_new_code(args...);
        }
        // Helper function to convert a float to a byte array
        std::vector<byte> floatToBytes(float value);
        std::vector<byte> intToBytes(int value);

        int m_nop;
        bool m_is_nop;
        void* m_value;
        void* m_target_address;
        std::unique_ptr<byte[]> m_original_bytes;
        void* m_new_code_address;
        std::vector<byte> m_new_code;
        std::unique_ptr<byte_patching> m_patch;
    };
}