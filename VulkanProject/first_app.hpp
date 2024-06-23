#pragma once

#include "lve_window.hpp"
#include "lve_pipeline.hpp"
#include "lve_swap_chain.hpp"
#include "lve_device.hpp"
#include "lve_model.hpp"

// std
#include <memory>

namespace lve {
	class FirstApp {
	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;

		FirstApp();
		~FirstApp();

		FirstApp(const FirstApp&) = delete;
		FirstApp& operator=(const FirstApp&) = delete;

		void run();

	private:
		void sierpinski(std::vector<LveModel::Vertex>& vertices,
			int depth,
			glm::vec2 left,
			glm::vec2 right,
			glm::vec2 top,
			glm::vec3 colorLeft,
			glm::vec3 colorRight,
			glm::vec3 colorTop);
		void loadModels();
		void createPipelineLayout();
		void createPipeline();
		void createCommandBuffers();
		void freeCommandBuffers();
		void drawFrame();
		void recreateSwapChain();
		void recordCommandBuffer(int imageIndex);

		LveWindow lveWindow{ WIDTH, HEIGHT, "Hello, Vulkan!" };
		LveDevice lveDevice{ lveWindow };
		std::unique_ptr<LveSwapChain> lveSwapChain;
		std::unique_ptr<LvePipeLine> lvePipeline;
		VkPipelineLayout pipelineLayout;
		std::vector<VkCommandBuffer> commandBuffers;
		std::unique_ptr<LveModel> lveModel;
	};
}