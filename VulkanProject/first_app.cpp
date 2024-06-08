#include "first_app.hpp"
#include <iostream>
#include <stdexcept>
#include <array>
namespace lve {

	FirstApp::FirstApp() {
		loadModels();
		createPipelineLayout();
		createPipeline();
		createCommandBuffers();
	}

	FirstApp::~FirstApp() {
		vkDestroyPipelineLayout(lveDevice.device(), pipelineLayout, nullptr);
	}
	void FirstApp::run() {
		while (!lveWindow.ShouldClose()) {
			glfwPollEvents();
			drawFrame();
		}

		vkDeviceWaitIdle(lveDevice.device());
	}

	void FirstApp::sierpinski(
		std::vector<LveModel::Vertex>& vertices,
		int depth,
		glm::vec2 left,
		glm::vec2 right,
		glm::vec2 top,
		glm::vec3 colorLeft,
		glm::vec3 colorRight,
		glm::vec3 colorTop) {

		if (depth <= 0) {
			vertices.push_back({ left, colorLeft });
			vertices.push_back({ right, colorRight });
			vertices.push_back({ top, colorTop });
		}
		else {
			auto leftTop = 0.5f * (left + top);
			auto rightTop = 0.5f * (right + top);
			auto leftRight = 0.5f * (left + right);

			auto colorLeftTop = 0.5f * (colorLeft + colorTop);
			auto colorRightTop = 0.5f * (colorRight + colorTop);
			auto colorLeftRight = 0.5f * (colorLeft + colorRight);

			sierpinski(vertices, depth - 1, left, leftRight, leftTop, colorLeft, colorLeftRight, colorLeftTop);
			sierpinski(vertices, depth - 1, leftRight, right, rightTop, colorLeftRight, colorRight, colorRightTop);
			sierpinski(vertices, depth - 1, leftTop, rightTop, top, colorLeftTop, colorRightTop, colorTop);
		}
	}

	void FirstApp::loadModels() {
		std::vector<LveModel::Vertex> vertices{};
		glm::vec2 left = { -1.0f, 1.0f };
		glm::vec2 right = { 1.0f, 1.0f };
		glm::vec2 top = { 0.0f, -1.0f };

		// Set base colors for the vertices to create a gradient
		glm::vec3 colorLeft = { 1.0f, 0.0f, 0.0f }; // Red
		glm::vec3 colorRight = { 0.0f, 1.0f, 0.0f }; // Green
		glm::vec3 colorTop = { 0.0f, 0.0f, 1.0f }; // Blue

		sierpinski(vertices, 3, left, right, top, colorLeft, colorRight, colorTop);
		lveModel = std::make_unique<LveModel>(lveDevice, vertices);
	}
	void FirstApp::createPipelineLayout() {
		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 0;
		pipelineLayoutInfo.pSetLayouts = nullptr;
		pipelineLayoutInfo.pushConstantRangeCount = 0;
		pipelineLayoutInfo.pPushConstantRanges = nullptr;
		if (vkCreatePipelineLayout(lveDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) !=
			VK_SUCCESS) {
			throw std::runtime_error("Failed to create pipeline layout!");
		}
	}
	void FirstApp::createPipeline() {
		auto pipelineConfig = LvePipeLine::defaultPipelineConfigInfo(lveSwapChain.width(), lveSwapChain.height());
		pipelineConfig.renderPass = lveSwapChain.getRenderPass();
		pipelineConfig.pipelineLayout = pipelineLayout;
		lvePipeline = std::make_unique<LvePipeLine>(
			lveDevice,
			"shaders/simple_shader.vert.spv",
			"shaders/simple_shader.frag.spv",
			pipelineConfig);
	}

	void FirstApp::createCommandBuffers() {

		commandBuffers.resize(lveSwapChain.imageCount());

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = lveDevice.getCommandPool();
		allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());
		if (vkAllocateCommandBuffers(lveDevice.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
			throw std::runtime_error("failed to allocate command buffers!");
		}

		for (int i = 0; i < commandBuffers.size(); i++) {
			std::cout << i << '\n';
			VkCommandBufferBeginInfo beginInfo{};
			beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

			if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS) {
				throw std::runtime_error("failed to begin recording command buffer!");
			}

			VkRenderPassBeginInfo renderPassInfo{};
			renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			renderPassInfo.renderPass = lveSwapChain.getRenderPass();
			renderPassInfo.framebuffer = lveSwapChain.getFrameBuffer(i);

			renderPassInfo.renderArea.offset = { 0, 0 };
			renderPassInfo.renderArea.extent = lveSwapChain.getSwapChainExtent();

			std::array<VkClearValue, 2> clearValues{};
			clearValues[0].color = { 0.1f, 0.1f, 0.1f, 1.0f };
			clearValues[1].depthStencil = { 1.0f, 0 };
			renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
			renderPassInfo.pClearValues = clearValues.data();

			vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

			lvePipeline->bind(commandBuffers[i]);
			lveModel->bind(commandBuffers[i]);
			lveModel->draw(commandBuffers[i]);

			vkCmdEndRenderPass(commandBuffers[i]);
			if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS) {
				throw std::runtime_error("failed to record command buffer!");
			}
		}
	}
	void FirstApp::drawFrame() {
		uint32_t imageIndex;
		auto result = lveSwapChain.acquireNextImage(&imageIndex);
		if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
			throw std::runtime_error("failed to acquire swap chain image!");
		}

		result = lveSwapChain.submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
	}

}