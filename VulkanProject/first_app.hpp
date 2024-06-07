#pragma once

#include "lve_window.hpp"
#include "lve_pipeline.hpp"
#include "lve_device.hpp"

namespace lve {
	class FirstApp {
	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;

		void run();

	private:
		LveWindow lveWindow{ WIDTH, HEIGHT, "Hello, Vulkan!" };
		LveDevice lveDevice{ lveWindow };
		LvePipeLine lvePipeline{ 
			lveDevice, 
			"shaders/simple_shader.vert.spv", 
			"shaders/simple_shader.frag.spv", 
			LvePipeLine::defaultPipelineConfigInfo(WIDTH, HEIGHT)};
	};
}