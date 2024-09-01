#pragma once

#include "server/socket_client.hpp"

namespace big
{
	class alpha_module
	{
	public:
		alpha_module()
		{
			socket_client_ = std::make_shared<socket_client>("server.imost.id/ws/alpha");
		}

		std::shared_ptr<socket_client> getClient()
		{
			return socket_client_;
		}

	private:
		std::shared_ptr<socket_client> socket_client_;
	};
}