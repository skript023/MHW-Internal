#pragma once
#include "alpha/alpha_service.hpp"
#include "alpha/alpha_module.hpp"

namespace big
{
    class server_module;
    inline server_module* g_server_module{};

    class server_module
    {
    public:
        explicit server_module()
        {
            g_server_module = this;
        }

        ~server_module() noexcept
        {
            g_server_module->shutdown();
        }

        std::unique_ptr<alpha_service>& get_alpha() { return m_alpha_service; }

        void run()
        {
            m_alpha_service->poll();
        }
    private:
        void init()
        {
            alpha_module alpha;
            m_alpha_service = std::make_unique<alpha_service>(alpha);
            m_alpha_service->ping();
        }

        void shutdown()
        {
            m_alpha_service.reset();

            g_server_module = nullptr;
        }
    private:
        std::unique_ptr<alpha_service> m_alpha_service;
    };
}