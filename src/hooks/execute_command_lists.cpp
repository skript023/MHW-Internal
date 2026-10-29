#include "hooking.hpp"
#include "renderer.hpp"

namespace big
{
	void APIENTRY hooks::execute_command_lists(ID3D12CommandQueue* queue, UINT num_command_lists, ID3D12CommandList* const* pp_command_lists)
	{
		if (queue && queue->GetDesc().Type == D3D12_COMMAND_LIST_TYPE_DIRECT)
		{
			if (g_renderer)
				g_renderer->set_command_queue(queue);
		}

		base_hook::get_original<hooks::execute_command_lists>()(queue, num_command_lists, pp_command_lists);
	}
}
